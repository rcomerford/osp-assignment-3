#include "allocator.h"

allocator::allocator(
    const strategy STR,
    const list<size_t> CS
){
    allocated_chunks = list<allocation*>();
    free_chunks = list<allocation*>();
    STRATEGY = STR;
    CHUNK_SIZES = CS;
    INITIAL_BRK_ADDRESS = sbrk(0);
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

        // create new allocation object
        allocation* new_alloc = new allocation();
        new_alloc->partition_size = new_partition_size;
        new_alloc->used_size = chunk_size;
        new_alloc->space = sbrk(new_partition_size);

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
        }),
        allocated_chunks.end()
    );
}

void allocator::output()
{
    cout << endl;

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
}