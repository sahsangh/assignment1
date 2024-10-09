CC = gcc
CFLAGS = -g -Wall

all: memgrind memtest test1 test2 test3 test4 test5 coalesce_test

mymalloc: mymalloc.c
	$(CC) $(CFLAGS) -o mm mymalloc.c

memgrind: mymalloc.c memgrind.c
	$(CC) $(CFLAGS) -c mymalloc.c
	$(CC) $(CFLAGS) -o memgrind memgrind.c mymalloc.o

memtest: mymalloc.c memtest.c
	$(CC) $(CFLAGS) -c mymalloc.c
	$(CC) $(CFLAGS) -o memtest memtest.c mymalloc.o

test1: mymalloc.c test1.c
	$(CC) $(CFLAGS) -c mymalloc.c
	$(CC) $(CFLAGS) -o test1 test1.c mymalloc.o


test2: mymalloc.c test2.c
	$(CC) $(CFLAGS) -c mymalloc.c
	$(CC) $(CFLAGS) -o test2 test2.c mymalloc.o


test3: mymalloc.c test3.c
	$(CC) $(CFLAGS) -c mymalloc.c
	$(CC) $(CFLAGS) -o test3 test3.c mymalloc.o


test4: mymalloc.c test4.c
	$(CC) $(CFLAGS) -c mymalloc.c
	$(CC) $(CFLAGS) -o test4 test4.c mymalloc.o

test5: mymalloc.c test5.c
	$(CC) $(CFLAGS) -c mymalloc.c
	$(CC) $(CFLAGS) -o test5 test5.c mymalloc.o

coalesce_test: mymalloc.c coalesce_test.c
	$(CC) $(CFLAGS) -c mymalloc.c
	$(CC) $(CFLAGS) -o coalesce_test coalesce_test.c mymalloc.o
clean:
	rm -f mymalloc.o memgrind memtest test1 test2 test3 test4 test5 coalesce_test