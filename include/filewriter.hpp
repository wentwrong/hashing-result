#ifndef FILEWRITER_HPP
#define FILEWRITER_HPP

#include <iostream>
#include <fstream>

class FileWriter
{
    std::ofstream ofs;
public:
    FileWriter(std::string fn);
    void writeToFile(std::string content);
};

#endif // FILEWRITER_HPP
