
CC=g++
CFLAGS=-std=C++11

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)
simulate: *.o
	$(CC) -o simulate $(CFLAGS) *.o
clean:
	rm -f *.o
