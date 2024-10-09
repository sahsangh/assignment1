#include <stdio.h>
#include <stdlib.h> // For rand(), srand()
#include <sys/time.h>
#include <time.h> // For time() used with srand()
#include "mymalloc.h"

#define RUNS 50

// Task 1: malloc() and immediately free 1-byte object 120 times
void task1()
{
    for (int i = 0; i < 120; i++)
    {
        void *ptr = malloc(1);
        free(ptr);
    }
}

// Task 2: malloc 120 1-byte objects and free them all
void task2()
{
    void *pointers[120];
    for (int i = 0; i < 120; i++)
    {
        pointers[i] = malloc(1);
    }
    for (int i = 0; i < 120; i++)
    {
        free(pointers[i]);
    }
}

// Task 3: Randomly malloc or free 120 1-byte objects
void task3()
{
    void *pointers[120];
    int allocated = 0;
    for (int i = 0; i < 240; i++)
    {
        if (allocated < 120 && (rand() % 2 == 0))
        {
            pointers[allocated++] = malloc(1); // Allocate if possible
        }
        else if (allocated > 0)
        {
            free(pointers[--allocated]); // Free if there are allocated blocks
        }
    }
    // Free any remaining allocations
    while (allocated > 0)
    {
        free(pointers[--allocated]);
    }
}

// Task 4: Allocate 50 64-byte objects and do not free (memory leak test)
void task4()
{
    void *pointers[50];
    for (int i = 0; i < 50; i++)
    {
        pointers[i] = malloc(64); // Allocate 64-byte objects
        if (pointers[i] == NULL)
        {
            printf("Unable to allocate object %d\n", i);
            exit(1);
        }
    }
    // Do not free the allocated memory (for leak testing)
}

// Task 5: Allocate and free 60 1-byte objects, free half, and reallocate
void task5()
{
    void *pointers[60];

    // Allocate 60 1-byte objects
    for (int i = 0; i < 60; i++)
    {
        pointers[i] = malloc(1);
    }

    // Free half of the objects
    for (int i = 0; i < 30; i++)
    {
        free(pointers[i]);
    }

    // Reallocate 30 objects
    for (int i = 0; i < 30; i++)
    {
        pointers[i] = malloc(1);
    }

    // Free remaining objects
    for (int i = 30; i < 60; i++)
    {
        free(pointers[i]);
    }
}

// Time a specific task and print the result
void time_task(void (*task)(), const char *task_name)
{
    struct timeval start, end;
    gettimeofday(&start, NULL); // Start time

    for (int i = 0; i < RUNS; i++)
    {
        task(); // Run the task
        reset_heap();
    }

    gettimeofday(&end, NULL); // End time
    long seconds = (end.tv_sec - start.tv_sec);
    long microseconds = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    long avg_time = microseconds / RUNS;
    printf("%s: %ld microseconds (avg over %d runs)\n", task_name, avg_time, RUNS);
}

int main()
{
    srand(time(NULL)); // Initialize random number generator

    printf("Testing custom malloc and free implementations...\n");

    // Time and run tasks
    time_task(task1, "Task 1 (malloc/free 1-byte object 120 times)");
    time_task(task2, "Task 2 (malloc 120 objects, then free them all)");
    time_task(task3, "Task 3 (random malloc/free)");

    time_task(task4, "Task 4 (malloc 64-byte objects, no free)");
    time_task(task5, "Task 5 (allocate 60, free half, reallocate)");
    return 0;
}
