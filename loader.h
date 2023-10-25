#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

#define ALLOC_STRING   "alloc:"
#define DEALLOC_STRING "dealloc"
#define LINE_DELIM     ' '

using std::string;
using std::pair;
using std::fstream;
using std::ios;
using std::cerr;
using std::endl;
using std::vector;
using std::stringstream;
using std::getline;

enum command 
{   
    EXIT = 0,
    DEALLOC = 1,
    ALLOC = 2
};

class loader
{
    private:

        /**
         * Input stream for the open file.
        */
        fstream data_file;

        /**
         * Validates if a string contains a valid number.
        */
        bool isNumber(const string& STR);

        /**
         * Splits a string by the given deliminator.
        */
        vector<string> splitString(
            const string& STR, 
            const char DELIM
        );

    public:

        /**
         * Con/destructor.
        */
        loader(const string& FILE_NAME);
        ~loader();

        /**
         * Returns a pair of <command, size_t> to be dealt with by loader.
         * If command is allocate, the size_t denotes the size of the data.
         * Otherwise, size_t will be zero and the last address will be used by main.
         * Once file is read, return EXIT.
        */
        pair<command, size_t> getNextCommand();

};

#endif // LOADER_H