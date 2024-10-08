#include <stdio.h>
#include "mymalloc.h"

int main()
{
    // Allocate two chunks of memory
    char *ptr1 = malloc(16);
    if (ptr1 == NULL)
    {
        printf("Failed to allocate Chunk 1!\n");
        return 1;
    }

    char *ptr2 = malloc(16);
    if (ptr2 == NULL)
    {
        printf("Failed to allocate Chunk 2!\n");
        return 1;
    }

    char *ptr3 = malloc(16);
    if (ptr3 == NULL)
    {
        printf("Failed to allocate Chunk 2!\n");
        return 1;
    }

    printf("Freeing Chunk 2...\n");
    free(ptr2);

    char *ptr4 = malloc(32);
    if (ptr4 == NULL)
    {
        printf("Failed to allocate Chunk 2!\n");
        return 1;
    }

    // Free the newly allocated chunk
    printf("Freeing the 64-byte block...\n");
    free(ptr3);

    return 0;
}
