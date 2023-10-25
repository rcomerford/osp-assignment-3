#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "allocation.h"
#include <list>
#include <cstdlib>
#include <new>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <ranges>

using std::list;
using std::size_t;
using std::cout;
using std::cerr;
using std::endl;
using std::for_each;

enum strategy 
{
    FIRST_FIT = 0,
    BEST_FIT = 1
};

class allocator
{
    private:

        /**
         * Allocation strategy to use.
        */
        strategy STRATEGY;

        /**
         * List of valid chunk sizes.
        */
        list<size_t> CHUNK_SIZES;

        /**
         * Initial address of the heap frontier. 
        */
        void* INITIAL_BRK_ADDRESS;

        /**
         * Linked list of allocated memory blocks.
        */
        list<allocation*> allocated_chunks;

        /**
         * List of deallocated (free) chunks.
        */
        list<allocation*> free_chunks;

    public:

        /**
         * Con/destructor.
        */
        allocator(
            const strategy STR,
            const list<size_t> CS
        );
        ~allocator();

        /**
         * Allocates a chunk of memory of the given size.
        */
        void* alloc(size_t chunk_size);

        /**
         * Deallocates the partition at a given location.
         * Note: does not return memory to OS, but allows for alloc().
        */
        void dealloc(void* chunk);

        /**
         * Prints:
         * - Linked list of occupied chunks & for each chunk:
         *      - Address
         *      - Total size
         *      - Used size 
         * - Linked list of free chunks & for each chunk:
         *      - Address
         *      - Total size
        */
        void output();
        
};

#endif // ALLOCATOR_H