CC = g++
FLAGS = -Wall -Wextra

test: clean main
	./main

main: main.cpp
	$(CC) $(FLAGS) -o main main.cpp

clean:
	rm main
