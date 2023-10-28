#include "allocator.h"

allocator::allocator(
    const strategy STR,
    const list<size_t> CS
){
    allocated_chunks = list<allocation*>();
    free_chunks = list<allocation*>();
    STRATEGY = STR;
    CHUNK_SIZES = CS;

    // get current location of the  heap frontier and error check
    INITIAL_BRK_ADDRESS = sbrk(0);
    
    if(INITIAL_BRK_ADDRESS == (void *)-1)
    {
        cerr << "Fatal Error:\tUnable to get initial heap frontier value." << endl;
        exit(EXIT_FAILURE);
    }
}

allocator::~allocator()
{
    // deallocate all items in lists
    for(auto &free_chunk : free_chunks)
        delete free_chunk;

    for(auto &alloc_chunk : allocated_chunks)
        delete alloc_chunk;

    free_chunks.clear();
    allocated_chunks.clear();

    // free all memory by resetting heap frontier
    brk(INITIAL_BRK_ADDRESS);
}

void* allocator::alloc(
    size_t chunk_size
){
    // validate chunk size
    if(chunk_size > CHUNK_SIZES.back())
    {
        cerr << "Fatal Error:\tChunk size (" << chunk_size << ") is too large." << endl;
        exit(EXIT_FAILURE);
    }

    list<allocation*>::iterator temp_iter;

    // find chunk based on allocations strategy
    if(STRATEGY == FIRST_FIT)
    {
        // look for a chunk big enough that is already allocated
        temp_iter = find_if(
            free_chunks.begin(), 
            free_chunks.end(), 
            [&](allocation* a) { 
                return a->partition_size >= chunk_size; 
            }
        );
    }
    else // if(STRATEGY == BEST_FIT)
    {
        long unsigned int curr_distance = SIZE_MAX;
        void* chunk_space = nullptr;

        // look for a the chunk closest to the given size
        for(auto &a : free_chunks)
        {
            if(
                a->partition_size >= chunk_size &&
                (a->partition_size - chunk_size) < curr_distance
            ){
                chunk_space = a->space;
            }
        }

        // look for a the chunk with the found address
        // used here in order to get the iterator as opposed to just the index
        temp_iter = find_if(
            free_chunks.begin(), 
            free_chunks.end(), 
            [&](allocation* a) { 
                return a->space == chunk_space; 
            }
        );
    }

    void* alloc_ptr = nullptr;

    // if large enough chunk was found use it
    if(temp_iter != free_chunks.end())
    {
        // change used size and return pointer value
        (*temp_iter)->used_size = chunk_size;
        alloc_ptr = (*temp_iter)->space;

        // add to back of free chunks
        allocated_chunks.push_back(*temp_iter);

        // remove from allocated chunks
        free_chunks.erase(
            remove_if(
                free_chunks.begin(), 
                free_chunks.end(),
                [&](allocation* a) { 
                    return a->space == (*temp_iter)->space; 
            }),
            free_chunks.end()
        );
    }
    // otherwise expand heap and create new chunk
    else
    {
        unsigned new_partition_size;

        // find the closest value in CHUNK_SIZES
        for(auto &valid_chunk_size : CHUNK_SIZES)
        {
            if(chunk_size < valid_chunk_size)
            {
                new_partition_size = valid_chunk_size;
                break;
            }
        }

        // expand heap frontier
        void* new_partition_space = sbrk(new_partition_size);

        if(new_partition_space == (void *)-1)
        {
            cerr << "Fatal Error:\tUnable to expand allocation of size " << new_partition_size << "." << endl;
            exit(EXIT_FAILURE);
        }

        // create new allocation object
        allocation* new_alloc = new allocation();
        new_alloc->partition_size = new_partition_size;
        new_alloc->used_size = chunk_size;
        new_alloc->space = new_partition_space;

        allocated_chunks.push_back(new_alloc);
        alloc_ptr = new_alloc->space;
    }

    return alloc_ptr;
}

void allocator::dealloc(
    void* chunk
){
    // find element matching address of chunk
    list<allocation*>::iterator temp_iter = find_if(
        allocated_chunks.begin(), 
        allocated_chunks.end(), 
        [&](allocation* a) { 
            return a->space == chunk; 
        }
    );

    // validate chunk was found
    if(temp_iter == allocated_chunks.end())
    {
        cerr << "Fatal Error:\tChunk \"" << (void*)chunk << "\" not found." << endl;
        exit(EXIT_FAILURE);
    }

    // add to back of free chunks
    free_chunks.push_back(*temp_iter);

    // remove from allocated chunks
    allocated_chunks.erase(
        remove_if(
            allocated_chunks.begin(), 
            allocated_chunks.end(),
            [&](allocation* a) { 
                return a->space == chunk; 
        })
    );
}

void allocator::output()
{
    cout << "----- FREE CHUNK LIST -----" << endl;
    for(auto &free_chunk : free_chunks)
    {
        cout 
            << "Address: "    << (void*)free_chunk->space   << " | "
            << "Total Size: " << free_chunk->partition_size
        << endl;
    }

    cout << endl;

    cout << "----- ALLOCATED CHUNK LIST -----" << endl;
    for(auto &alloc_chunk : allocated_chunks)
    {
        cout 
            << "Address: "    << (void*)alloc_chunk->space   << " | "
            << "Total Size: " << alloc_chunk->partition_size << " | "
            << "Used Size: "  << alloc_chunk->used_size
        << endl;
    }

    cout << endl;

    // calculate performance metrics

    size_t free_chunks_bytes = 0;
    size_t occupied_chunks_bytes = 0;
    size_t total_chunks_bytes = 0;
    size_t allocated_used_bytes = 0;
    size_t allocated_unused_bytes = 0;

    for(auto &a : free_chunks)
        free_chunks_bytes += a->partition_size;

    for(auto &a : allocated_chunks)
        occupied_chunks_bytes += a->partition_size;

    for(auto &a : allocated_chunks)
        allocated_used_bytes += a->used_size;

    for(auto &a : allocated_chunks)
        allocated_unused_bytes += (a->partition_size - a->used_size);

    total_chunks_bytes = free_chunks_bytes + occupied_chunks_bytes;

    cout << "----- PERFORMANCE METRICS -----" << endl;
    cout << "Free Chunk Bytes:\t"       << free_chunks_bytes << endl;
    cout << "Allocated Chunk Bytes:\t"  << occupied_chunks_bytes << endl;
    cout << "Total Chunk Bytes:\t"      << total_chunks_bytes << endl;
    cout << "Allocated Used Bytes:\t"   << allocated_used_bytes << endl;
    cout << "Allocated Unused Bytes:\t" << allocated_unused_bytes << endl;

    cout << endl;
}