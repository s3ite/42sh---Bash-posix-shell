CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -pedantic -lcriterion
LDFLAGS=-fsanitize=address
SRC = ../src/*/*.c Tests_redirection.c

check: 
	$(CC) $(CFLAGS) $(SRC) -o check

checkleaks: 
	$(CC) $(CFLAGS) $(LDFLAGS)  $(SRC) -o check

clean:
	rm *.o check
