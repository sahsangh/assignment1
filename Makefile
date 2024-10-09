CC = gcc
CFLAGS = -g -Wall

all: memgrind memtest

mymalloc: mymalloc.c
	$(CC) $(CFLAGS) -o mm mymalloc.c

memgrind: mymalloc.c memgrind.c
	$(CC) $(CFLAGS) -c mymalloc.c
	$(CC) $(CFLAGS) -o memgrind memgrind.c mymalloc.o

memtest: mymalloc.c memtest.c
	$(CC) $(CFLAGS) -c mymalloc.c
	$(CC) $(CFLAGS) -o memtest memtest.c mymalloc.o
clean:
	rm -f mymalloc.o memgrind memtest