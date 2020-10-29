#ifndef __SCRYPT_H__
#define __SCRYPT_H__
#include <vector>

std::vector<uint32_t> Salsa20_8 (std::vector<uint32_t>);

std::vector<std::vector<uint32_t>> ScryptBlockMix (std::vector<std::vector<uint32_t>>, int);

std::vector<std::vector<uint32_t>> ScryptROMix (std::vector<std::vector<uint32_t>>, int, uint32_t);

#endif // __SCRYPT_H__