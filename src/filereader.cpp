#include "filereader.hpp"

FileReader::FileReader(std::string fn)
{
    fs.open(fn, std::ifstream::binary);

    if(not fs) {
        std::cerr << "Can't open a file " << fn << std::endl;
        exit(1);
    }

    try {
        // C++17
        this->file_size = std::filesystem::file_size(fn);
    } catch (const std::filesystem::filesystem_error& err) {
        std::cerr << "Filesystem error: " << err.what() << std::endl;
        exit(1);
    }
}

long FileReader::read_at(char *buffer, unsigned long offset, unsigned long num_of_bytes)
{
    std::lock_guard<std::mutex> locker(read_mutex);
    if(not fs)
        // !!!
        return 0;
    fs.seekg(long(offset));
    fs.read(buffer, long(num_of_bytes));
    return fs.gcount();
}

unsigned long FileReader::get_size()
{
    return this->file_size;
}
