# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Executables
TARGET = memgrind

# Source files
SRC = memgrind.c mymalloc.c

# Object files
OBJ = memgrind.o mymalloc.o

# Default target to build all
all: $(TARGET)

# Rule to compile memgrind
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Compile memgrind.c
memgrind.o: memgrind.c mymalloc.h
	$(CC) $(CFLAGS) -c memgrind.c

# Compile mymalloc.c
mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -c mymalloc.c

# Clean rule to remove compiled files
clean:
	rm -f $(TARGET) *.o