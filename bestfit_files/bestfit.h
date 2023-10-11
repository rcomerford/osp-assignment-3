#include "allocator.h"

class bestfit : public allocator
{

    private:

        /* data */

    public:

        /**
         * Con/destructor.
        */
        bestfit();
        ~bestfit();

        /**
         * Allocated a memory partition of size 32, 64, 128, 256, or 512 bytes.
         * Using the best fit memory allocation scheme.
         * Returns a pointer to the memory chunk.
        */
        void* alloc(size_t chunk_size);

};