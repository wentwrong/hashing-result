#include "argparse.hpp"

ArgParse::ArgParse(int argc, char* argv[])
{
    if(argc < 3) {
        std::cerr << "Incorrect number of parameters." << std::endl
                  << "Usage: ./hashing-result input.txt output.txt 1024" << std::endl;
        exit(1);
    }

    this->input_fn = argv[1];
    this->output_fn = argv[2];

    if(argc >= 3) {
        try {
            this->chunk_size = std::stoul(argv[3]);
        } catch (...) {
            this->chunk_size = DEFAULT_CHUNK_SIZE;
        }
    } else
        this->chunk_size = DEFAULT_CHUNK_SIZE;
}

std::string ArgParse::getInputFn() {
    return this->input_fn;
}

std::string ArgParse::getOutputFn() {
    return this->output_fn;
}

size_t ArgParse::getChunkSize() {
    return this->chunk_size;
}
