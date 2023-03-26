CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror -Wvla

all: bistro

main: bistro
	$(CC) $(CFLAGS) -o main *.o main.c -L. bistro.a

bistro:
	$(CC) $(CFLAGS) ./src/*.c -c
	ar csr bistro.a *.o
	
debug:
	$(CC) $(CFLAGS) -g -o debug src/*.c main.c

clean:
	 rm -rf $(wildcard *.o) $(wildcard *.a) bistro testsuite debug main

testsuite:
	$(CC) $(CFLAGS) -g -lcriterion -o testsuite src/*.c tests/*.c
	./testsuite
