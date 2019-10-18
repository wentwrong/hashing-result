#include "filewriter.hpp"

FileWriter::FileWriter(std::string fn)
{
    ofs.open(fn, std::ofstream::out | std::ofstream::trunc);

    if(not ofs) {
        std::cerr << "Can't create a file " << fn << std::endl;
        exit(1);
    }
}

void FileWriter::writeToFile(std::string content)
{
    ofs << content;
}
