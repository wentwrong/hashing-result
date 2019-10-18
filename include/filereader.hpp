#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

class FileReader
{
    std::ifstream fs;
    long file_size;
    long chunk;
public:
    FileReader(std::string fn, size_t chunk_size);
    size_t getFileSize();
    std::unique_ptr<std::vector<char>> nextChunk();
    bool isEof();
};

#endif // FILEREADER_HPP
