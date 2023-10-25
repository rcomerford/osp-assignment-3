#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <cstdlib>

using std::size_t;

/**
 * A partition of allocated memory.
*/
struct allocation
{ 

    /**
     * Max partition size, 32/64/128/256/512 bytes.
    */
    size_t partition_size; 

    /**
     * Used size of the data.
    */
    size_t used_size; 

    /**
     * Heap starting location allocated by sbrk().
    */
    void *space;

};

#endif // ALLOCATION_H