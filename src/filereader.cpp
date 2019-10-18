#include "filereader.hpp"

FileReader::FileReader(std::string fn, size_t chunk_size) : chunk(chunk_size)
{
    fs.open(fn, std::ifstream::binary);

    if(not fs) {
        std::cerr << "Can't open a file " << fn << std::endl;
        exit(1);
    }
}

std::unique_ptr<std::vector<char>> FileReader::nextChunk() {
    std::unique_ptr<std::vector<char>> ret(new std::vector<char>(chunk));
    fs.read(ret->data(), long(chunk));
    ret->resize(fs.gcount());
    return ret;
}

size_t FileReader::getFileSize() {
    return this->file_size;
}

bool FileReader::isEof() {
    return fs.eof();
}
