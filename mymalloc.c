#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h> // For uint64_t
#include "mymalloc.h"

#define MEMLENGTH 104

static union
{
    char bytes[MEMLENGTH];
    double align;
} heap;

typedef struct chunk_header
{
    uint64_t metadata; // 8 bytes: size in higher bits, is_free in the lowest bit
} chunk_header;

static int heap_initialized = 0;
static void *heap_start = NULL;

void init_heap();
chunk_header *find_free_chunk(size_t size);
void split_chunk(chunk_header *chunk, size_t size);
void coalesce();
void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);
void print_heap();

// Get size from upper bits
size_t get_size(chunk_header *chunk)
{
    return chunk->metadata & ~1ULL;
}

void set_size(chunk_header *chunk, size_t size)
{
    chunk->metadata = (chunk->metadata & 1) | size; // Keep the is_free bit and set the new size
}

bool is_free(chunk_header *chunk)
{
    return chunk->metadata & 1; // Check if the lowest bit is 1 (free)
}

void set_free(chunk_header *chunk, bool free)
{
    if (free)
    {
        chunk->metadata |= 1; // Set the lowest bit to 1
    }
    else
    {
        chunk->metadata &= ~1ULL; // Set the lowest bit to 0
    }
}

// Leak detection function that checks for unfreed memory at program exit
void leak_detector()
{
    chunk_header *current = (chunk_header *)heap_start;
    size_t total_leaked = 0;
    size_t leaked_chunks = 0;

    while ((char *)current < heap.bytes + MEMLENGTH)
    {
        if (!is_free(current))
        {
            leaked_chunks++;
            total_leaked += get_size(current) - sizeof(chunk_header);
        }
        current = (chunk_header *)((char *)current + get_size(current));
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
    initial_header->metadata = MEMLENGTH | 1;
    print_heap();
    heap_initialized = 1;
    atexit(print_heap);
    atexit(leak_detector); // Register leak detector to run at program exit
}

// Find the first free chunk large enough to hold the requested size
chunk_header *find_free_chunk(size_t size)
{
    chunk_header *ptr = (chunk_header *)heap_start;
    while ((char *)ptr < heap.bytes + MEMLENGTH)
    {
        if (is_free(ptr) && get_size(ptr) >= size)
        {
            return ptr;
        }
        ptr = (chunk_header *)((char *)ptr + get_size(ptr));
    }
    return NULL;
}

// Split a large chunk into two smaller chunks
void split_chunk(chunk_header *chunk, size_t size)
{
    size_t remaining_size = get_size(chunk) - size;
    if (remaining_size >= sizeof(chunk_header) + 8)
    {
        chunk_header *new_chunk = (chunk_header *)((char *)chunk + size);
        set_size(new_chunk, remaining_size);
        set_free(new_chunk, true);

        set_size(chunk, size);
    }
}

// Memory allocation function
void *mymalloc(size_t size, char *file, int line)
{

    if (size == 0)
    {
        fprintf(stderr, "malloc: Requested size is 0 (%s:%d)\n", file, line);
        return NULL;
    }

    if (!heap_initialized)
    {
        init_heap();
    }

    // Round up the requested size to the nearest multiple of 8 (alignment)
    size_t payload_size = (size + 7) & ~7;

    // Add header size to the total size
    size_t total_size = payload_size + sizeof(chunk_header);
    // Find a free chunk that can accommodate the total size
    chunk_header *free_chunk = find_free_chunk(total_size);
    if (!free_chunk)
    {
        fprintf(stderr, "malloc: Unable to allocate %zu bytes (%s:%d)\n", size, file, line);
        return NULL;
    }

    // Split Chunk if it is larger than the requested size
    if (get_size(free_chunk) > total_size)
    {
        split_chunk(free_chunk, total_size);
    }

    // Mark chunk as allocated
    set_free(free_chunk, false);
    print_heap();
    // Return a pointer to the payload (NOT HEADER)
    return (void *)((char *)free_chunk + sizeof(chunk_header));
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
    if (is_free(chunk))
    {
        fprintf(stderr, "free: Double free detected (%s:%d)\n", file, line);
        exit(2);
    }

    // Mark the chunk as free
    set_free(chunk, true);

    // Coalesce adjacent free chunks
    coalesce();
    print_heap();
}

// Coalesce adjacent free chunks to avoid fragmentation
void coalesce()
{
    chunk_header *current = (chunk_header *)heap_start;
    while ((char *)current < heap.bytes + MEMLENGTH)
    {
        chunk_header *next = (chunk_header *)((char *)current + get_size(current));
        if ((char *)next < heap.bytes + MEMLENGTH && is_free(current) && is_free(next))
        {
            set_size(current, get_size(current) + get_size(next)); // Merge adjacent free chunks
        }
        else
        {
            current = next; // Move to the next chunk
        }
    }
}

// Print the current state of the heap
void print_heap()
{
    chunk_header *current = (chunk_header *)heap_start;
    int chunk_number = 0;

    printf("\nHeap State:\n");
    while ((char *)current < heap.bytes + MEMLENGTH)
    {
        printf("Chunk %d: Address: %p, Size: %zu, Status: %s\n", chunk_number, (void *)current, get_size(current), is_free(current) ? "Free" : "Allocated");
        current = (chunk_header *)((char *)current + get_size(current));
        chunk_number++;
    }
    printf("\n");
}

// Macros to replace malloc and free with our custom functions
// #define malloc(s) mymalloc(s, __FILE__, __LINE__)
// #define free(p) myfree(p, __FILE__, __LINE__)
