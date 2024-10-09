#include <stdio.h>
#include "mymalloc.h"

int main()
{
    // Allocate two chunks of memory
    char *ptr1 = malloc(40);
    if (ptr1 == NULL)
    {
        printf("Failed to allocate Chunk 1!\n");
        return 1;
    }

    char *ptr2 = malloc(40);
    if (ptr2 == NULL)
    {
        printf("Failed to allocate Chunk 2!\n");
        return 1;
    }
    free(ptr1);
    free(ptr2);
    char *ptr3 = malloc(72);
    if (ptr3 == NULL)
    {
        printf("Failed to allocate Chunk 2!\n");
        return 1;
    }

    if (ptr1 == ptr3)
    {
        printf("SUCCESS");
    }
    else
    {
        printf("FAIL");
    }
    free(ptr3);

    return 0;
}
