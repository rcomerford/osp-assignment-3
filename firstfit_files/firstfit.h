#include "allocator.h"

class firstfit : public allocator
{

    private:

        /* data */

    public:

        /**
         * Con/destructor.
        */
        firstfit();
        ~firstfit();

        /**
         * Allocated a memory partition of size 32, 64, 128, 256, or 512 bytes.
         * Using the first fit memory allocation scheme.
         * Returns a pointer to the memory chunk.
        */
        void* alloc(size_t chunk_size);

};