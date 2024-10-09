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
#define OBJSIZE ((MEMSIZE / OBJECTS - HEADERSIZE) / 8 * 8 + 8) // Ensure OBJSIZE is a multiple of 8

int main(int argc, char **argv)
{
	char *obj[OBJECTS];
	int i, j, errors = 0;

	// fill memory with objects
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
	// fill each object with distinct bytes
	for (i = 0; i < OBJECTS; i++)
	{
		memset(obj[i], i, OBJSIZE);
	}
	// check that all objects contain the correct bytes
	for (i = 0; i < OBJECTS; i++)
	{
		for (j = 0; j < OBJSIZE; j++)
		{
			if (obj[i][j] != i)
			{
				errors++;
				printf("Object %d byte %d incorrect: %d\n", i, j, obj[i][j]);
			}
		}
	}

	// free all objects
	if (!LEAK)
	{
		for (i = 0; i < OBJECTS; i++)
		{
			free(obj[i]);
		}
	}

	printf("%d incorrect bytes\n", errors);

	// Additional tests to fulfill all requirements
	// Test malloc() to ensure objects do not overlap
	char *obj1 = malloc(OBJSIZE);
	char *obj2 = malloc(OBJSIZE);
	if (obj1 == NULL || obj2 == NULL)
	{
		printf("Unable to allocate additional objects\n");
		exit(1);
	}

	// Check alignment for obj1 and obj2
	if ((uintptr_t)obj1 % 8 != 0)
	{
		printf("Error: Allocated object 1 is not aligned to 8 bytes\n");
		errors++;
	}
	if ((uintptr_t)obj2 % 8 != 0)
	{
		printf("Error: Allocated object 2 is not aligned to 8 bytes\n");
		errors++;
	}

	if (obj1 + OBJSIZE > obj2 || obj2 + OBJSIZE > obj1)
	{
		printf("Error: Allocated objects overlap\n");
		errors++;
	}

	// Fill and check non-overlapping allocation
	memset(obj1, 1, OBJSIZE);
	memset(obj2, 2, OBJSIZE);
	for (j = 0; j < OBJSIZE; j++)
	{
		if (obj1[j] != 1)
		{
			errors++;
			printf("Object 1 byte %d incorrect: %d\n", j, obj1[j]);
		}
		if (obj2[j] != 2)
		{
			errors++;
			printf("Object 2 byte %d incorrect: %d\n", j, obj2[j]);
		}
	}

	// Free objects and check coalescing
	free(obj1);
	free(obj2);
	char *obj3 = malloc(2 * OBJSIZE); // Should fit into the coalesced space of obj1 and obj2
	if (obj3 == NULL)
	{
		printf("Error: Unable to allocate coalesced memory\n");
		errors++;
	}
	else
	{
		// Check alignment for obj3
		if ((uintptr_t)obj3 % 8 != 0)
		{
			printf("Error: Allocated object 3 is not aligned to 8 bytes\n");
			errors++;
		}

		memset(obj3, 3, 2 * OBJSIZE);
		for (j = 0; j < 2 * OBJSIZE; j++)
		{
			if (obj3[j] != 3)
			{
				errors++;
				printf("Object 3 byte %d incorrect: %d\n", j, obj3[j]);
			}
		}
		free(obj3);
	}

	// Test for memory leaks (only applicable if LEAK is not defined)
	if (!LEAK)
	{
		// Allocate and deliberately not free to check for leak detection
		char *leak_test = malloc(OBJSIZE);
		if (leak_test == NULL)
		{
			printf("Error: Unable to allocate memory for leak test\n");
			errors++;
		}
		else
		{
			// Check alignment for leak_test
			if ((uintptr_t)leak_test % 8 != 0)
			{
				printf("Error: Allocated leak_test object is not aligned to 8 bytes\n");
				errors++;
			}
		}
		// Memory should be leaked here (no free for leak_test)
	}

	// Test for freeing the same memory twice
	char *double_free_test = malloc(OBJSIZE);
	if (double_free_test == NULL)
	{
		printf("Error: Unable to allocate memory for double free test\n");
		errors++;
	}
	else
	{
		free(double_free_test);
		// Attempt to free the same memory again
		free(double_free_test);
		printf("Attempted to free memory twice (check for double free error reporting)\n");
	}

	// Test for freeing a null pointer
	char *null_free_test = NULL;
	free(null_free_test); // Should not cause any error
	printf("Freed a null pointer (check for no error in freeing nothing)\n");

	// Final test results
	printf("Final test results: %d incorrect bytes\n", errors);

	return EXIT_SUCCESS;
}