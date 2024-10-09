#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
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
#define OBJSIZE 53

int main(int argc, char **argv)
{
    char *obj[OBJECTS];
    int i, errors = 0;

    // Test case: 8-byte alignment verification
    for (i = 0; i < OBJECTS; i++)
    {
        obj[i] = malloc(OBJSIZE);
        if (obj[i] == NULL)
        {
            printf("Unable to allocate object %d\n", i);
            exit(1);
        }

        // Check that the returned pointer is aligned to a multiple of 8
        if ((uintptr_t)obj[i] % 8 != 0)
        {
            printf("Error: Allocated object %d is not aligned to 8 bytes\n", i);
            errors++;
        }
    }
    printf("\n\n\nErrors:%d\n\n\n", errors);
}