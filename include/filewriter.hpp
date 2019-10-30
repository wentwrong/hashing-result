#ifndef FILEWRITER_HPP
#define FILEWRITER_HPP

#include <iostream>
#include <fstream>
#include <mutex>

class FileWriter
{
    std::ofstream fs;
    std::mutex write_mutex;
public:
    FileWriter(std::string fn);
    void write_at(std::string hash_value, unsigned long offset);
};

#endif // FILEWRITER_HPP
