// MAKE SURE EVERYTHING IS MULTIPLE OF 8
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Compile with -DREALMALLOC to use the real malloc() instead of mymalloc()
#ifndef REALMALLOC
#include "mymalloc.h"
#endif

// Compile with -DLEAK to leak memory
#ifndef LEAK
#define LEAK 0
#endif

#define MEMSIZE 4096
#define HEADERSIZE 8
#define OBJECTS 64
#define OBJSIZE 56 // Ensure OBJSIZE is a multiple of 8

int main(int argc, char **argv)
{
    int *p = malloc(sizeof(int) * 2);
    free(p + 1);
}