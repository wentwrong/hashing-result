#ifndef CRC32HASHER_HPP
#define CRC32HASHER_HPP

#include <ihasher.hpp>

class CRC32Hasher : public IHasher
{
public:
    std::string getHash(unsigned char *buf, size_t len) override;
};

#endif // CRC32HASHER_HPP
