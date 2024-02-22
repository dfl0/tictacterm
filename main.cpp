#include <iostream>

#define CLEAR std::cout << "\e[1;1H\e[2J"
#define COL_DEF "\033[0m"
#define COL_DIM "\033[90m"
#define COL_RED "\033[31m"
#define COL_GRN "\033[92m"

int board[3][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};

void print_color(std::string content, std::string col) {
    std::cout << col << content << COL_DEF;
}
void print_board() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (j > 0) std::cout << "│";
            switch(board[i][j]) {
                case 1:
                    std::cout << COL_GRN << " X " << COL_DEF;
                    break;
                case -1:
                    std::cout << COL_RED << " O " << COL_DEF;
                    break;
                default:
                    std::cout << COL_DIM << ' ' << std::to_string(i * 3 + j + 1) << ' ' << COL_DEF;
                    break;
            }
        }
        if (i != 2) std::cout << "\n───┼───┼───\n";
    }
    std::cout << "\n";
}

int check_win() {
    int row;
    int col;
    int sum_diag1 = 0;
    int sum_diag2 = 0;
    for (row = 0; row < 3; ++row) {
        int sum_row = 0;
        int sum_col = 0;
        for (col = 0; col < 3; ++col) {
            sum_row += board[row][col];
            sum_col += board[col][row];

            if (row == col) sum_diag1 += board[row][col];
            if (row + col == 2) sum_diag2 += board[row][col];
        }
        if (sum_row == 3 || sum_col == 3 || sum_diag1 == 3 || sum_diag2 == 3) return 1;
        if (sum_row == -3 || sum_col == -3 || sum_diag1 == -3 || sum_diag2 == -3) return -1;
    }

    // check for tie
    for (row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col)
            if (board[row][col] == 0) return 0; // still empty space on the board

    return 2; // board is full, resulting in a tie
}

void play_line(int line, int player) {
    if (line < 3) {
        // block in the empty spot in the winning row
        for (int col = 0; col < 3; ++col)
            if (board[line][col] == 0) board[line][col] = player;
    } else if (line >= 3 && line < 6) {
        // block in the empty spot in the winning col
        for (int row = 0; row < 3; ++row)
            if (board[row][line - 3] == 0) board[row][line - 3] = player;
    } else {
        // block in the empty spot in the winning diagonal
        if (line == 6) {
            for (int row = 0; row < 3; ++row)
                for (int col = 0; col < 3; ++col) 
                    if (row == col && board[row][col] == 0) board[row][col] = player;
        }
        if (line == 7) {
            for (int row = 0; row < 3; ++row)
                for (int col = 0; col < 3; ++col) 
                    if (row + col == 2 && board[row][col] == 0) board[row][col] = player;
        }
    }
}

void cpu_move(int player) {
    if (board[1][1] == 0) {
        // always go to the middle if first move
        board[1][1] = player;
        return;
    }

    int win_line = -1;
    int block_line = -1;
    // check if either player is one move away from winning on any row
    for (int row = 0; row < 3; ++row) {
        int sum_row = 0;
        for (int col = 0; col < 3; ++col) sum_row += board[row][col];
        if (sum_row == player * 2) win_line = row;
        if (sum_row == -player * 2) block_line = row;
    }

    // check if either player is one move away from winning on any col
    for (int col = 0; col < 3; ++col) {
        int sum_col = 0;
        for (int row = 0; row < 3; ++row) sum_col += board[row][col];
        if (sum_col == player * 2) win_line = col + 3;
        if (sum_col == -player * 2) block_line = col + 3;
    }

    // check if either player is one move away from winning on any diagonal
    int sum_diag1 = 0;
    int sum_diag2 = 0;
    for (int row = 0; row < 3; ++row) {
        sum_diag1 += board[row][row];
        sum_diag2 += board[row][2 - row];
        if (sum_diag1 == player * 2) win_line = 6;
        if (sum_diag2 == player * 2) win_line = 7;
        if (sum_diag1 == -player * 2) block_line = 6;
        if (sum_diag2 == -player * 2) block_line = 7;
    }

    // std::cout << "line to win: " << win_line << "\n";
    // std::cout << "line to block: " << block_line << "\n";
    
    if (win_line != -1) {
        play_line(win_line, player);
        return;
    } else if (block_line != -1) {
        play_line(block_line, player);
        return;
    }

    int corners[4][2] = {{0, 0}, {0, 2}, {2, 0}, {2, 2}};
    int edges[4][2] = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    for (int i = 0; i < 4; ++i) {
        int row = corners[i][0];
        int col = corners[i][1];
        if (board[row][col] == 0) {
            board[row][col] = player;
            // std::cout << "made a random corner move\n";
            return;
        }
    }

    for (int i = 0; i < 4; ++i) {
        int row = edges[i][0];
        int col = edges[i][1];
        if (board[row][col] == 0) {
            board[row][col] = player;
            // std::cout << "made a random edge move\n";
            return;
        }
    }
}

int two_player(int first) {
    int player = first;
    int pos;
    while (true) {
        char team = player == 1 ? 'X' : 'O'; // set player symbol: turn 1 represents X, -1 represenst O

        print_board();
        std::cout << '\n' << team << "'s, where would you like to place your next move?\n";
        std::cout << "Position (1-9): ";
        std::cin >> pos;

        CLEAR;
        int row = (pos - 1) / 3;
        int col = (pos - 1) % 3;
        // check if position is valid
        if (board[row][col] == 0) {
            board[row][col] = player;
        } else {
            std::cout << "Please select a valid position:\n\n";
            continue;
        }

        std::cout << team << "'s made their move:\n\n";
        if (check_win() != 0) return check_win();
        player = player == 1 ? -1 : 1; // switch to other player's turn
    }
}

int one_player() {
    int player = 1; // X's move first
    int pos;
    while (true) {
        char team = player == 1 ? 'X' : 'O'; // set player team: turn 1 represents X, O represenst 0

        print_board();
        if (player == 1) {
            std::cout << '\n' << team << "'s, where would you like to place your next move?\n";
            std::cout << "Position (1-9): ";
            std::cin >> pos;

            CLEAR;
            int row = (pos - 1) / 3;
            int col = (pos - 1) % 3;
            // check if position is valid
            if (board[row][col] == 0) {
                board[row][col] = player;
            } else {
                std::cout << "Please select a valid position:\n\n";
                continue;
            }

            std::cout << "You made your move:\n\n";
        } else {
            cpu_move(player);
            CLEAR;
            std::cout << "CPU made its move:\n\n";
        }

        if (check_win() != 0) return check_win();
        player = player == 1 ? -1 : 1; // switch to other player's turn
    }
}

int main() {
    int players;

    CLEAR;
    std::cout << "Tic Tac Term\n\n";
    std::cout << "1 or 2 Players?\n:";

    while (1) {
        std::cin >> players;
        CLEAR;
        if (players != 1 && players != 2) {
            std::cout << "Tic Tac Term\n\n";
            std::cout << "Please specify either 1 or 2 players!\n:";
        } else {
            break;
        }
    }

    std::cout << "Tic Tac Term\n\n";
    int outcome = players == 1 ? one_player() : two_player(1); // start game with correct # of players

    switch (outcome) {
        case 1:
            print_board();
            std::cout << "\nX's won!\n";
            break;
        case -1:
            print_board();
            std::cout << "\nO's won!\n";
            break;
        case 2:
            print_board();
            std::cout << "\nIt's a tie!\n";
            break;
    }

    return 0;
}
