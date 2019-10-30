#include "hasher.hpp"

Hasher::Hasher()
{

}

std::string Hasher::get_hash(unsigned char *buf, unsigned long len)
{
    unsigned long crc_table[256];
    unsigned long crc;
    for (unsigned long i = 0; i < 256; i++)
    {
        crc = i;
        for (int j = 0; j < 8; j++)
            crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
        crc_table[i] = crc;
    }
    crc = 0xFFFFFFFFUL;
    while (len--)
        crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);

    std::string ans = std::to_string(crc ^ 0xFFFFFFFFUL);
    // padding leading zeros
    return std::string(10 - ans.length(), '0') + ans;
}
