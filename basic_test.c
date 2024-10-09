#include <stdio.h>
#include <string.h>
#include "mymalloc.h"

int main()
{
    // Basic memory allocation test
    printf("Allocating 32 bytes of memory...\n");
    char *ptr = malloc(32);

    if (ptr == NULL)
    {
        printf("Failed to allocate memory!\n");
        return 1;
    }
    else
    {
        printf("Task 1 Successful\n\n\n\n");
    }

    // Basic memory re-allocation test
    printf("Allocating 32 bytes of memory...\n");
    char *ptr2 = malloc(32);

    if (ptr2 == NULL)
    {
        printf("Failed to allocate memory!\n");
        return 1;
    }

    // Write some data to the newly allocated memory
    strcpy(ptr2, "Testing reallocation!");
    printf("Stored string: %s\n", ptr2);

    // Free the allocated memory again
    printf("Freeing reallocated memory...\n");
    free(ptr2);

    return 0;
}
