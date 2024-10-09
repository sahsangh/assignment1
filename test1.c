// TESTS BASIC MALLOC AND FREE AND MALLOC(0) and MALLOC(4096) AND DOUBLE FREE AND FREEING NON MALLOC ADDRESS
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
#define OBJSIZE (MEMSIZE / OBJECTS - HEADERSIZE)

int main()
{
    // BASIC MALLOC FREE TEST
    char *obj[OBJECTS];

    for (int i = 0; i < OBJECTS; i++)
    {
        obj[i] = malloc(OBJSIZE);
        if (obj[i] == NULL)
        {
            printf("Unable to allocate object %d\n", i);
            exit(1);
        }
        free(obj[i]);
    }

    char *ptr = malloc(0);
    char *ptr2 = malloc(4097);

    // DOUBLE FREE
    char *ptr3 = malloc(4);
    free(ptr3);
    free(ptr3);

    // FREE NON MALLOC
    int x;
    free(&x);
}
