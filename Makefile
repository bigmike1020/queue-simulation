
CC=g++
CFLAGS=-std=c++0x -Wall -Werror -Wfatal-errors

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)
simulate: *.o
	$(CC) -o simulate $(CFLAGS) *.o
clean:
	rm -f *.o
