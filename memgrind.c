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

// Time a specific task and print the result
void time_task(void (*task)(), const char *task_name)
{
    struct timeval start, end;
    gettimeofday(&start, NULL); // Start time

    for (int i = 0; i < RUNS; i++)
    {
        task(); // Run the task
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

    return 0;
}
