
CC=g++
CFLAGS=-std=c++0x -Wall -Werror -Wfatal-errors

default: simulate

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)
simulate: *.o
	$(CC) -o simulate $(CFLAGS) *.o

.PHONY:clean

clean:
	rm -f *.o
