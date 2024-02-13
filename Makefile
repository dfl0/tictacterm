CC= g++
CFLAGS= -Wall -Wextra -g

main: main.o
	$(CC) $(FLAGS) main.o -o $@

main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp -o $@

clean:
	rm main && rm *.o
