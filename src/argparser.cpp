#include "argparser.hpp"

ArgParser::ArgParser(int argc, char *argv[])
{
    if(argc < 3) {
        std::cerr << "Incorrect number of parameters." << std::endl
                  << "Usage: ./hashing-result-mt input output 1024" << std::endl;
        exit(1);
    }
    this->input_fn = argv[1];
    this->output_fn = argv[2];
    if(argc >= 3) {
        try {
            this->block_size = std::stoul(argv[3]);
        } catch (...) {
            this->block_size = DEFAULT_BLOCK_SIZE;
        }
    } else
        this->block_size = DEFAULT_BLOCK_SIZE;
}

std::string ArgParser::get_input_fn()
{
    return this->input_fn;
}

std::string ArgParser::get_output_fn()
{
    return this->output_fn;
}

unsigned long ArgParser::get_block_size()
{
    return this->block_size;
}
