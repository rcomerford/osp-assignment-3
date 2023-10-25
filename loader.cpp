#include "loader.h"

loader::loader(
    const string& FILE_NAME
){
    data_file.open(FILE_NAME, ios::in);

    if(!data_file.is_open())
    {
        cerr << "Fatal Error:\tUnable to open file \"" << FILE_NAME << "\"." << endl;
        exit(EXIT_FAILURE);
    }
}

loader::~loader()
{
    if(data_file.is_open())
        data_file.close();
}

bool loader::isNumber(const string& STR)
{
    bool result = true;
    int size = STR.length();

    for(int i = 0; i < size; i++) 
    {
        char ch = STR[i];
        if(!isdigit(ch)) 
            result = false;
    }

    return result;
}

vector<string> loader::splitString(
    const string& STR, 
    const char DELIM
){
    vector<string> result;
    stringstream ss(STR);
    string item;

    while(getline(ss, item, DELIM))
        result.push_back (item);

    return result;
}

pair<command, size_t> loader::getNextCommand()
{
    pair<command, size_t> result;

    // get next line
    string line;
    if(getline(data_file, line))
    { 
        // split string by spaces
        vector<string> string_split = splitString(line, LINE_DELIM);

        // detect line and run alloc/dealloc
        if(
            string_split.size() == 2 && 
            string_split.at(0) == ALLOC_STRING && 
            isNumber(string_split.at(1))
        ){
            // ALLOCATE
            size_t chunk_size = stoi(string_split.at(1));
            result = pair(ALLOC, chunk_size);
        } 
        else if(string_split.at(0) == DEALLOC_STRING)
        {
            // DEALLOCATE
            result = pair(DEALLOC, (size_t)0);
        }
        else
        {
            cerr << "Fatal Error:\tUnable to process input at line \"" << line << "\"." << endl;
            exit(EXIT_FAILURE);
        }
    } 
    else
    {
        result = pair(EXIT, (size_t)0);
    }

    return result;
}