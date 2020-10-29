#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>
#include <cstdint>
#include "utils.hpp"

std::vector<std::vector<std::vector<uint32_t> > > buffer_to_triple_array(uint32_t, uint32_t, uint32_t, uint8_t*);

uint8_t* triple_array_to_buffer(std::vector<std::vector<std::vector<uint32_t>>>);

char* pad (char *);

char* unpad (char *);

#endif // __UTILS_H__