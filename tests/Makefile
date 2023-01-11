CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -pedantic -fsanitize=address -lcriterion
SRC = ../src/*/*.c Tests.c

check: 
	$(CC) $(CFLAGS) $(SRC) -o check

clean:
	rm *.o check
