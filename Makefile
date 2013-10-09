
CC=g++
CFLAGS=-std=c++0x -Os -Wall -Werror -Wfatal-errors

default: simulate

simulate: *.cpp
	$(CC) -o simulate $(CFLAGS) *.cpp

.PHONY: clean

clean:
	rm -f *.o simulate
