#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "mymalloc.h"

#define MEMLENGTH 4096

static union
{
    char bytes[MEMLENGTH];
    double not_used; // To ensure 8-byte alignment
} heap;

typedef struct chunk_header
{
    size_t size;  // Total size of the chunk (including the header)
    bool is_free; // Is the chunk free or allocated
} chunk_header;

static int heap_initialized = 0;
static void *heap_start = NULL;

// Function prototypes
void init_heap();
chunk_header *find_free_chunk(size_t size);
void split_chunk(chunk_header *chunk, size_t size);
void coalesce();
void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);
void print_heap();

// Leak detection function that checks for unfreed memory at program exit
void leak_detector()
{
    chunk_header *current = (chunk_header *)heap_start;
    size_t total_leaked = 0;
    size_t leaked_chunks = 0;

    while ((char *)current < heap.bytes + MEMLENGTH)
    {
        if (!current->is_free)
        {
            leaked_chunks++;
            total_leaked += current->size;
        }
        current = (chunk_header *)((char *)current + current->size);
    }

    if (leaked_chunks > 0)
    {
        fprintf(stderr, "mymalloc: %zu bytes leaked in %zu objects.\n", total_leaked, leaked_chunks);
    }
}
// Initialize the heap on first allocation
void init_heap()
{
    heap_start = (void *)heap.bytes;
    chunk_header *initial_header = (chunk_header *)heap_start;
    initial_header->size = MEMLENGTH;
    initial_header->is_free = true;
    heap_initialized = 1;
    print_heap();
    atexit(print_heap);
    atexit(leak_detector); // Register leak detector to run at program exit
}

chunk_header *find_free_chunk(size_t size)
{
    chunk_header *ptr = (chunk_header *)heap_start;
    while ((char *)ptr < heap.bytes + MEMLENGTH)
    {
        if (ptr->is_free && ptr->size >= size)
        {
            return ptr;
        }
        ptr = (chunk_header *)((char *)ptr + ptr->size);
    }
    return NULL;
}

void split_chunk(chunk_header *chunk, size_t size)
{
    size_t remaining_size = chunk->size - size;
    if (remaining_size >= sizeof(chunk_header) + 8)
    {
        chunk_header *new_chunk = (chunk_header *)((char *)chunk + size);
        new_chunk->size = remaining_size;
        new_chunk->is_free = true;

        chunk->size = size;
    }
}

// Memory allocation function
void *mymalloc(size_t size, char *file, int line)
{
    // Handle size == 0 case
    if (size == 0)
    {
        fprintf(stderr, "malloc: Requested size is 0 (%s:%d)\n", file, line);
        return NULL;
    }

    // Initialize the heap if not already initialized
    if (!heap_initialized)
    {
        init_heap();
    }

    // Round up the requested size to the nearest multiple of 8 (alignment)
    size_t total_size = (size + 7) & ~7;
    if (total_size < 16)
    {
        total_size = 16;
    }

    // Find a free chunk that can accommodate the total size
    chunk_header *free_chunk = find_free_chunk(total_size);
    if (!free_chunk)
    {
        fprintf(stderr, "malloc: Unable to allocate %zu bytes (%s:%d)\n", size, file, line);
        return NULL;
    }

    // If the chunk is larger than the requested size, split it
    if (free_chunk->size > total_size)
    {
        split_chunk(free_chunk, total_size);
    }

    // Mark the chunk as allocated
    free_chunk->is_free = false;
    print_heap();
    //    Return a pointer to the payload (skip over the header)
    return (void *)((char *)free_chunk + sizeof(chunk_header));
}

// Coalesce adjacent free chunks to avoid fragmentation
void coalesce()
{
    chunk_header *current = (chunk_header *)heap_start;
    while ((char *)current < heap.bytes + MEMLENGTH)
    {
        chunk_header *next = (chunk_header *)((char *)current + current->size);
        if ((char *)next < heap.bytes + MEMLENGTH && current->is_free && next->is_free)
        {
            current->size += next->size; // Merge adjacent free chunks
        }
        else
        {
            current = next; // Move to the next chunk
        }
    }
}

// Free memory and coalesce adjacent free chunks
void myfree(void *ptr, char *file, int line)
{
    // Null pointer check
    if (ptr == NULL)
    {
        fprintf(stderr, "free: Inappropriate pointer (%s:%d)\n", file, line);
        return;
    }

    // Pointer validity check (ensure the pointer is within the bounds of the heap)
    if ((char *)ptr < heap.bytes || (char *)ptr >= heap.bytes + MEMLENGTH)
    {
        fprintf(stderr, "free: Pointer not in heap range (%s:%d)\n", file, line);
        exit(2);
    }

    // Calculate the corresponding chunk header
    chunk_header *chunk = (chunk_header *)((char *)ptr - sizeof(chunk_header));

    // Detect double free
    if (chunk->is_free)
    {
        fprintf(stderr, "free: Double free detected (%s:%d)\n", file, line);
        exit(2);
    }

    // Mark the chunk as free
    chunk->is_free = true;

    // Coalesce adjacent free chunks
    coalesce();
}

void print_heap()
{
    chunk_header *current = (chunk_header *)heap_start;
    int chunk_number = 0;

    printf("\nHeap State:\n");
    while ((char *)current < heap.bytes + MEMLENGTH)
    {
        printf("Chunk %d: Address: %p, Size: %zu, Status: %s\n", chunk_number, (void *)current, current->size, current->is_free ? "Free" : "Allocated");
        current = (chunk_header *)((char *)current + current->size);
        chunk_number++;
    }
    printf("\n");
}