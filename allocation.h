#include <cstdlib>

using std::size_t;

/**
 * A partition of allocated memory.
*/
struct allocation
{ 

    /**
     * Partition size, 32/64/128/256/512 bytes.
    */
    size_t size; 

    /**
     * Heap starting location allocated by sbrk().
    */
    void *space; 

};