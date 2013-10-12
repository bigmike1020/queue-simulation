
CC=g++
CFLAGS=-std=c++0x -Os -Wall

default: simulate

simulate: *.cpp *.h
	$(CC) -o simulate $(CFLAGS) *.cpp

.PHONY: clean

clean:
	rm -f *.o simulate
