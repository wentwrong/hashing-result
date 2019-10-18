#include <iostream>
#include <vector>
#include <memory>

#include <argparse.hpp>
#include <filereader.hpp>
#include <filewriter.hpp>
#include <ihasher.hpp>
#include <crc32hasher.hpp>

int main(int argc, char* argv[])
{
    ArgParse parser(argc, argv);

    FileReader fr(parser.getInputFn(), parser.getChunkSize());
    FileWriter fw(parser.getOutputFn());

    std::unique_ptr<IHasher> hasher(new CRC32Hasher());

    std::unique_ptr<std::vector<char>> chunk;

    do {
        chunk = fr.nextChunk();
        auto hash_value = hasher->getHash(reinterpret_cast<unsigned char*>(chunk->data()), chunk->size());
        // std::cout << hash_value << std::endl;
        fw.writeToFile(hash_value);
    } while (!fr.isEof());

    return 0;
}
