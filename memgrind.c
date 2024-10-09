#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
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

// Task 2: malloc 120 1-byte objects and then free them all
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
            pointers[allocated++] = malloc(1);
        }
        else if (allocated > 0)
        {
            free(pointers[--allocated]);
        }
    }
    while (allocated > 0)
    {
        free(pointers[--allocated]);
    }
}

// Task 4: Randomly allocate and free memory of different sizes
void task4()
{
    void *pointers[120];
    int allocated = 0;

    for (int i = 0; i < 240; i++)
    {
        if (allocated < 120 && (rand() % 2 == 0))
        {
            int size = (rand() % 6 + 1) * 4;
            pointers[allocated++] = malloc(size);
        }
        else if (allocated > 0)
        {
            free(pointers[--allocated]);
        }
    }

    while (allocated > 0)
    {
        free(pointers[--allocated]);
    }
}

// Task 5: Test fragmentation and coalescing
void task5()
{
    int num_small_blocks = 50;
    int small_block_size = 16;
    int large_block_size = 128;
    void *pointers[num_small_blocks];

    for (int i = 0; i < num_small_blocks; i++)
    {
        pointers[i] = malloc(small_block_size);
        if (pointers[i] == NULL)
        {
            printf("Allocation failed at block %d\n", i);
            exit(1);
        }
    }

    for (int i = 10; i < 20; i++)
    {
        free(pointers[i]);
    }
    for (int i = 30; i < 40; i++)
    {
        free(pointers[i]);
    }

    void *large_block = malloc(large_block_size);
    void *large_block2 = malloc(large_block_size);
    if (pointers[10] != large_block || pointers[30] != large_block2)
    {
        printf("coalescing did not work properly");
    }
    if (large_block == NULL || large_block2 == NULL)
    {
        printf("Unable to allocate larger block after coalescing.\n");
        exit(1);
    }

    free(large_block);
    free(large_block2);
    for (int i = 0; i < num_small_blocks; i++)
    {
        if (i < 10 || (i >= 20 && i < 30) || i >= 40)
        {
            free(pointers[i]);
        }
    }
}

// Time a specific task and print the result
void time_task(void (*task)(), const char *task_name)
{
    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < RUNS; i++)
    {
        task();
    }

    gettimeofday(&end, NULL);
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
    time_task(task1, "Task 1: malloc/free 1-byte object 120 times");
    time_task(task2, "Task 2: malloc 120 objects, then free them all");
    time_task(task3, "Task 3: random malloc/free of 1-byte objects");
    time_task(task4, "Task 4: randomly allocate and free varying-sized objects");
    time_task(task5, "Task 5: fragmentation and coalescing test");

    return 0;
}
