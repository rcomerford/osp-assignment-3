#include "allocator.h"
#include "loader.h"

#include <stack>
#include <chrono>

using std::stack;
using std::chrono::high_resolution_clock;
using std::chrono::microseconds;

int main(
    int argc, 
    char const *argv[]
){  
    // start timing
    auto start = high_resolution_clock::now();

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

    // print details
    alloc.output();

    // calculate runtime
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    cout << "----- RUNTIME -----" << endl;
    cout << "Total Time:\t" << duration.count() << "ms" << endl;
    cout << endl;

    return EXIT_SUCCESS;
}
