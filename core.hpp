#ifndef __CORE_H__
#define __CORE_H__
#include <vector>

std::vector<uint32_t> Salsa20_8 (std::vector<uint32_t>);

std::vector<std::vector<uint32_t>> ScryptBlockMix (std::vector<std::vector<uint32_t>>, int);

std::vector<std::vector<uint32_t>> ScryptROMix (std::vector<std::vector<uint32_t>>, int, uint32_t);

unsigned char* generate_salt();

uint8_t* scrypt(const char *password, const char *salt, uint32_t N, int r, int p, int cLen);

#endif // __CORE_H__