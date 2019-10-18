#ifndef ARGPARSE_HPP
#define ARGPARSE_HPP

#include <iostream>

#include <constants.hpp>

class ArgParse
{
    std::string input_fn, output_fn;
    size_t chunk_size;
public:
    ArgParse(int argc, char* argv[]);
    size_t      getChunkSize();
    std::string getInputFn();
    std::string getOutputFn();
};

#endif // ARGPARSE_HPP
