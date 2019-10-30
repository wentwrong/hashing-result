#ifndef HASHER_HPP
#define HASHER_HPP

#include <string>

class Hasher
{
public:
    Hasher();
    static std::string get_hash(unsigned char *buf, unsigned long len);
};

#endif // HASHER_HPP
