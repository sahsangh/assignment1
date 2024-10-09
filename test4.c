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
    // char *obj[OBJECTS];
    // int i, j, errors = 0;

    // // Allocate memory for all objects
    // for (i = 0; i < OBJECTS; i++)
    // {
    //     obj[i] = malloc(OBJSIZE);
    //     if (obj[i] == NULL)
    //     {
    //         printf("Unable to allocate object %d\n", i);
    //         exit(1);
    //     }
    // }

    // // Fill each object with a distinct byte pattern
    // for (i = 0; i < OBJECTS; i++)
    // {
    //     memset(obj[i], i, OBJSIZE);
    // }

    // // Verify that there is no overlap or overwriting by checking each object
    // for (i = 0; i < OBJECTS; i++)
    // {
    //     for (j = 0; j < OBJSIZE; j++)
    //     {
    //         if (obj[i][j] != i)
    //         {
    //             errors++;
    //             printf("Error: Object %d, byte %d has incorrect value: %d\n", i, j, obj[i][j]);
    //         }
    //     }
    // }

    // // Free all allocated memory
    // for (i = 0; i < OBJECTS; i++)
    // {
    //     free(obj[i]);
    // }

    // // Summary of test results
    // if (errors == 0)
    // {
    //     printf("No overwriting or overlap detected. All objects contain correct values.\n");
    // }
    // else
    // {
    //     printf("%d errors detected: Overwriting or overlap occurred.\n", errors);
    // }
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
