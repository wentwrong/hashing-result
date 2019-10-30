#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include <iostream>
#include <fstream>
#include <mutex>
#include <filesystem>

class FileReader
{
    std::ifstream fs;
    std::mutex read_mutex;
    unsigned long file_size;
public:
    FileReader(std::string fn);
    long read_at(char* buffer, unsigned long offset, unsigned long num_of_bytes);
    unsigned long get_size();
};

#endif // FILEREADER_HPP
