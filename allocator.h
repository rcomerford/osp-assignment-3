#include "allocation.h"
#include <list>
#include <cstdlib>
#include <new>

using std::list;
using std::size_t;
using std::bad_alloc;

class allocator
{
    private:
        
        /**
         * List of allocated memory blocks.
         * Locations stores as allocation pointers.
         * Sizes accessed through allocation struct.
        */
        list<allocation*> allocations;

    public:

        /**
         * Con/destructor.
        */
        allocator();
        ~allocator();

        /**
         * Deallocates the partition at a given location.
         * Note: does not return memory to OS, but allows for allocation by alloc().
        */
        void dealloc(void* chunk);
        
};