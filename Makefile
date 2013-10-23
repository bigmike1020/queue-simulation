
CC=g++
CFLAGS=-std=c++0x -Wall

default: simulate

simulate: *.cpp *.h
	$(CC) -o simulate $(CFLAGS) *.cpp

.PHONY: clean

clean:
	rm -f *.o simulate
