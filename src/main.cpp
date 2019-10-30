#include <argparser.hpp>
#include <filereader.hpp>
#include <filewriter.hpp>
#include <threadpool.hpp>

int main(int argc, char* argv[])
{
    // Parsing input params
    // (input filename, output filename, [optional block size])
    ArgParser parser(argc, argv);

    // Thread-safe file reading/writing classes
    std::unique_ptr<FileReader> fr(new FileReader(parser.get_input_fn()));
    std::unique_ptr<FileWriter> fw(new FileWriter(parser.get_output_fn()));

    // Pool of threads (workers)
    ThreadPool thrpool(fr.get(), fw.get(), parser.get_block_size());

    // Running threads
    thrpool.run();

    // Wait until all threads finish their work
    thrpool.join_all();

    return 0;
}
