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
        if (i != 2) std::cout << std::endl << "───┼───┼───" << std::endl;
    }
    std::cout << std::endl;
}

int check_win() {
    int sum_diag1 = 0;
    int sum_diag2 = 0;
    for (int i = 0; i < 3; ++i) {
        int sum_rows = 0;
        int sum_cols = 0;
        for (int j = 0; j < 3; ++j) {
            sum_rows += board[i][j];
            sum_cols += board[j][i];

            if (i == j) sum_diag1 += board[i][j];
            if (i + j == 2) sum_diag2 += board[i][j];
        }
        if (sum_rows == 3 || sum_cols == 3 || sum_diag1 == 3 || sum_diag2 == 3) return 1;
        if (sum_rows == -3 || sum_cols == -3 || sum_diag1 == -3 || sum_diag2 == -3) return -1;
    }

    // check for tie
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[i][j] == 0) return 0; // still empty space on the board

    return 2; // board is full, resulting in a tie
}

int main() {
    int turn = 1; // X's move first
    int pos;
    bool playing = true;

    CLEAR;
    std::cout << "Tic Tac Toe\n\n";

    while (playing) {
        char player = turn == 1 ? 'X' : 'O'; // get character representation of current player

        print_board();
        std::cout << '\n' << player << "'s, where would you like to place your next move?\n";
        std::cout << "Position (1-9): ";
        std::cin >> pos;

        CLEAR;
        int row = (pos - 1) / 3;
        int col = (pos - 1) % 3;
        // check if position is valid
        if (board[row][col] == 0) {
            board[row][col] = turn;
        } else {
            std::cout << "Please select a valid position:\n\n";
            continue;
        }

        std::cout << player << "'s made their move:\n\n";
        if (check_win() != 0) break;
        turn = turn == 1 ? -1 : 1;
    }

    switch (check_win()) {
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
