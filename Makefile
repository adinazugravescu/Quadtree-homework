CC=gcc
CFLAGS=-Wall


build: tema2.c tema2.h
	$(CC) $(CFLAGS) tema2.c -lm -o quadtree

run: build
	./quadtree

clean:
	rm -f *.o exec
