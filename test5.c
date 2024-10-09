// MAKES SURE METADATA INTEGREITY REMAINS INTACT
#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

int main()
{
    printf("Running metadata integrity test...\n\n");

    void *ptr1 = malloc(16);
    void *ptr2 = malloc(32);
    void *ptr3 = malloc(24);

    if (ptr1 == NULL || ptr2 == NULL || ptr3 == NULL)
    {
        fprintf(stderr, "Failed to allocate memory\n");
        return 1;
    }

    printf("Allocated 16 bytes at %p\n", ptr1);
    printf("Allocated 32 bytes at %p\n", ptr2);
    printf("Allocated 24 bytes at %p\n\n", ptr3);

    // print_heap();

    printf("Freeing ptr2...\n\n");
    free(ptr2);

    // print_heap();

    printf("Freeing ptr1 and ptr3...\n\n");
    free(ptr1);
    free(ptr3);

    // print_heap();

    printf("Attempting to allocate 40 bytes...\n\n");
    void *ptr4 = malloc(40);

    if (ptr4 == NULL)
    {
        fprintf(stderr, "Failed to allocate memory\n");
        return 1;
    }

    printf("Allocated 40 bytes at %p\n\n", ptr4);
    // print_heap();

    free(ptr4);

    printf("Final heap state:\n\n");
    // print_heap();

    return 0;
}