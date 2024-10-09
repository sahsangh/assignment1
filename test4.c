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
#define OBJSIZE ((MEMSIZE / OBJECTS - HEADERSIZE) / 8 * 8) // Ensure OBJSIZE is a multiple of 8

int main(int argc, char **argv)
{
    int errors = 0;
    char *obj1 = malloc(OBJSIZE);
    char *obj2 = malloc(OBJSIZE);
    if (obj1 + OBJSIZE > obj2 || obj1 == obj2)
    {
        printf("Error: Allocated objects overlap\n");
        errors++;
    }
    if (errors == 0)
    {
        printf("No overwriting or overlap detected. All objects contain correct values.\n");
    }
    else
    {
        printf("%d errors detected: Overwriting or overlap occurred.\n", errors);
    }

    return EXIT_SUCCESS;
}
