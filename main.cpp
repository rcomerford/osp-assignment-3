#include "allocator.h"

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>

#define ALLOC_STRING   "alloc:"
#define DEALLOC_STRING "dealloc"
#define LINE_DELIM ' '

using std::fstream;
using std::string;
using std::getline;
using std::ios;
using std::vector;
using std::stringstream;
using std::stack;

/**
 * TODO
*/
stack<void*> allocations;

/**
 * TODO
*/
vector<string> splitString(
    const string &s, 
    char delim
){
    vector<string> result;
    stringstream ss(s);
    string item;

    while(getline(ss, item, delim))
        result.push_back (item);

    return result;
}

/**
 * Validates INTEGER command-line input.
*/
bool isNumber(const string& s)
{
    bool result = true;
    int size = s.length();

    for(int i = 0; i < size; i++) 
    {
        char ch = s[i];
        if(!isdigit(ch)) 
            result = false;
    }

    return result;
}

/**
 * TODO
*/
void handleLine(
    allocator* alloc,
    string& line
){
    // split string
    char delim = LINE_DELIM;
    vector<string> string_split = splitString(line, delim);

    // detect line and run alloc/dealloc
    if(
        string_split.size() == 2 && 
        string_split.at(0) == ALLOC_STRING && 
        isNumber(string_split.at(1))
    ){
        // ALLOCATE, pushing to the stack so they can be deallocated in order
        size_t chunk_size = stoi(string_split.at(1));
        allocations.push(alloc->alloc(chunk_size));
    } 
    else if(string_split.at(0) == DEALLOC_STRING)
    {
        // DEALLOCATE, removing the last allocated address
        alloc->dealloc(allocations.top());
        allocations.pop();
    }
    else
        cerr << "Error:\tUnable to process input at line \"" << line << "\"." << endl;
}

/**
 * TODO
*/
int main(
    int argc, 
    char const *argv[]
){
    // create allocator object
    allocator* alloc = new allocator();
    
    // input validation
    if(argc != 2)
    {
        cerr << "Fatal Error:\tProgram must be run in format ./[executable] [data_file]." << endl;
        return EXIT_FAILURE;
    } 

    fstream data_file;
    data_file.open(argv[1], ios::in);
    
    if(data_file.is_open())
    {
        string line;

        // read line from file and pass to handler
        while(getline(data_file, line))
        { 
            cout << line << endl; // TODO remove
            handleLine(alloc, line);
        }

        // close the file object.
        data_file.close();
    }
    else
    {
        cerr << "Fatal Error:\tUnable to open file \"" << argv[1] << "\"" << endl;
        return EXIT_FAILURE;
    }

    // print and exit
    alloc->output();
    delete alloc;

    return EXIT_SUCCESS;
}
