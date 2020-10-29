#include <vector>
#include <iostream>
#include <cstdint>
#include <cstring>
#include "utils.hpp"

std::vector<std::vector<std::vector<uint32_t> > > buffer_to_triple_array(uint32_t size0, uint32_t size1, uint32_t size2, uint8_t* buf) {
    std::vector<std::vector<std::vector<uint32_t> > > out(size0);
    for (uint32_t i = 0; i < size0; ++i) {
        out[i] = std::vector<std::vector<uint32_t> >(size1);
        for (uint32_t j = 0; j < size1; ++j) {
            out[i][j] = std::vector<uint32_t>((uint32_t*)(buf+i*size1*size2*4+j*size2*4), 
                                              (uint32_t*)(buf+i*size1*size2*4+(j+1)*size2*4));
        }
    }
    return out;
}

uint8_t* triple_array_to_buffer(std::vector<std::vector<std::vector<uint32_t> > > buf) {
    uint32_t size0 = buf.size();
    uint32_t size1 = size0 ? buf[0].size() : 0;
    uint32_t size2 = size0 && size1 ? buf[0][0].size() : 0;

    uint32_t* out = (uint32_t*)malloc(size0*size1*size2*4);
    for (uint32_t i = 0; i < size0; ++i) {
        for (uint32_t j = 0; j < size1; ++j) {
            for (uint32_t k = 0; k < size2; ++k) {
                out[i*size1*size2 + j*size2 + k] = buf[i][j][k];
            }
        }
    }
    return (uint8_t*)out;
}

char* pad (char *message) {
    int block_size = 16;
    int message_length = strlen(message);

    char padding = message_length % block_size;
    if (padding == 0) padding = block_size;

    char *padded_message = new char[message_length + padding];
    for (int i = 0; i < message_length; ++i) padded_message[i] = message[i];
    for (int i = 0; i < padding; ++i) padded_message[message_length + i] = padding;

    return padded_message;
}

char* unpad (char *padded_message) {
    int message_length = strlen(padded_message);
    char padding = padded_message[message_length - 1];

    char *message = new char[message_length - padding];
    for (int i = 0; i < message_length - padding; ++i) message[i] = padded_message[i];

    return message;
}