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
        if(command.first == ALLOC)
        {
            // allocate new memory, adding address to the stack
            void* new_pointer = alloc.alloc(command.second);
            allocations.push(new_pointer);
        }
        else if(command.first == DEALLOC)
        {
            // validate allocation on stack
            void* last_pointer;
            if(allocations.size() == 0)
            {
                cerr << "Warning:\tMore allocations than deallocations in file." << endl;
                last_pointer = nullptr;
            } 
            else
            {
                // if not empty, pop
                last_pointer = allocations.top();
            }
            
            // deallocate last address popped from the stack
            alloc.dealloc(last_pointer);
            allocations.pop();
        }
        else
        {
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
    cout << "Total Time:\t" << duration.count() << "μs" << endl;
    cout << endl;

    return EXIT_SUCCESS;
}
