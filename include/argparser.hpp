#ifndef ARGPARSER_HPP
#define ARGPARSER_HPP

#include <iostream>
#include <string>

#include <constants.hpp>

class ArgParser
{
    std::string input_fn, output_fn;
    unsigned long block_size;
public:
    ArgParser(int argc, char* argv[]);
    std::string get_input_fn();
    std::string get_output_fn();
    unsigned long get_block_size();
};

#endif // ARGPARSER_HPP
