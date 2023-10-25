#include "allocator.h"
#include "loader.h"

#include <stack>

using std::stack;

int main(
    int argc, 
    char const *argv[]
){  
    // input validation
    if(argc != 2)
    {
        cerr << "Fatal Error:\tProgram must be run in format ./[executable] [data_file]." << endl;
        return EXIT_FAILURE;
    }

    // generate arguments
    const list<size_t> CHUNK_SIZES = { 32, 64, 128, 256, 512 };
    const strategy STRATEGY = (strcmp(argv[0], "./firstfit") == 0) ? FIRST_FIT : BEST_FIT;

    // create instances
    allocator alloc = allocator(STRATEGY, CHUNK_SIZES);
    loader ld = loader(argv[1]);
    stack<void*> allocations;
    
    // get commands from file
    pair<command, size_t> command = ld.getNextCommand();
    while(command.first != EXIT)
    {
        switch(command.first) 
        {
            case ALLOC:
                // allocate new memory, adding address to the stack
                allocations.push(alloc.alloc(command.second));
                break;

            case DEALLOC:
                // deallocate last address popped from the stack
                alloc.dealloc(allocations.top());
                allocations.pop();
                break;

            default:
                cerr << "Warning:\tUnknown command returned by loader." << endl;
        }
        command = ld.getNextCommand();
    }

    // print and exit
    alloc.output();
    return EXIT_SUCCESS;
}
