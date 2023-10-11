#include "allocator.h"

allocator::allocator()
{
    // TODO make pointer?
    allocations = list<allocation*>();
}

allocator::~allocator()
{
    // TODO remove?
}

void allocator::dealloc(
    void* chunk
){
    // TODO implement
}
