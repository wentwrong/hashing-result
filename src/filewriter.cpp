#include "filewriter.hpp"

FileWriter::FileWriter(std::string fn)
{
    fs.open(fn, std::ios::out | std::ios::trunc);

    if(not fs) {
        std::cerr << "Can't create output file " << fn << std::endl;
        exit(1);
    }
}

void FileWriter::write_at(std::string hash_value, unsigned long offset)
{
    std::lock_guard<std::mutex> locker(write_mutex);
    if(not fs)
        // !!!
        return;
    fs.seekp(long(offset));
    fs << hash_value;
}
