// MAKE SURE EVERYTHING IS MULTIPLE OF 8
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifndef REALMALLOC
#include "mymalloc.h"
#endif

#ifndef LEAK
#define LEAK 0
#endif

#define MEMSIZE 4096
#define HEADERSIZE 8
#define OBJECTS 64
#define OBJSIZE 56

int main(int argc, char **argv)
{
    int *p = malloc(sizeof(int) * 2);
    free(p + 1);
}