#ifndef IHASHER_HPP
#define IHASHER_HPP

#include <string>

class IHasher
{
public:
    virtual std::string getHash(unsigned char *buf, size_t len) = 0;
};

#endif // IHASHER_HPP
