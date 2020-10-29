#include <openssl/evp.h>
 #include <openssl/rand.h>
#include <iostream>
#include <cstdint>
#include <vector>
#include <cstring>


uint32_t R(uint32_t x, uint8_t c) {
    return ((x << c) | (x >> (32-c)));
}

/**
 * Salsa20
 * ref: https://cr.yp.to/salsa20.html
 * 
 * @param[in] s vector representing the sequence of input words
 */
std::vector<uint32_t> Salsa20_8 (std::vector<uint32_t> s) {
    std::vector<uint32_t> out(s);
    int i;
    for (i = 0; i < 4; ++i) {
        out[ 4] ^= R(out[ 0]+out[12], 7);  out[ 8] ^= R(out[ 4]+out[ 0], 9);
        out[12] ^= R(out[ 8]+out[ 4],13);  out[ 0] ^= R(out[12]+out[ 8],18);
        out[ 9] ^= R(out[ 5]+out[ 1], 7);  out[13] ^= R(out[ 9]+out[ 5], 9);
        out[ 1] ^= R(out[13]+out[ 9],13);  out[ 5] ^= R(out[ 1]+out[13],18);
        out[14] ^= R(out[10]+out[ 6], 7);  out[ 2] ^= R(out[14]+out[10], 9);
        out[ 6] ^= R(out[ 2]+out[14],13);  out[10] ^= R(out[ 6]+out[ 2],18);
        out[ 3] ^= R(out[15]+out[11], 7);  out[ 7] ^= R(out[ 3]+out[15], 9);
        out[11] ^= R(out[ 7]+out[ 3],13);  out[15] ^= R(out[11]+out[ 7],18);
        out[ 1] ^= R(out[ 0]+out[ 3], 7);  out[ 2] ^= R(out[ 1]+out[ 0], 9);
        out[ 3] ^= R(out[ 2]+out[ 1],13);  out[ 0] ^= R(out[ 3]+out[ 2],18);
        out[ 6] ^= R(out[ 5]+out[ 4], 7);  out[ 7] ^= R(out[ 6]+out[ 5], 9);
        out[ 4] ^= R(out[ 7]+out[ 6],13);  out[ 5] ^= R(out[ 4]+out[ 7],18);
        out[11] ^= R(out[10]+out[ 9], 7);  out[ 8] ^= R(out[11]+out[10], 9);
        out[ 9] ^= R(out[ 8]+out[11],13);  out[10] ^= R(out[ 9]+out[ 8],18);
        out[12] ^= R(out[15]+out[14], 7);  out[13] ^= R(out[12]+out[15], 9);
        out[14] ^= R(out[13]+out[12],13);  out[15] ^= R(out[14]+out[13],18);
    }
    for (i = 0; i  < 16; ++i) out[i] += s[i];
    return out;
}

std::vector<std::vector<uint32_t>> ScryptBlockMix (std::vector<std::vector<uint32_t>> B, int r) {
    std::vector<uint32_t> y_moins_1(B[2*r -1]);
    std::vector<std::vector<uint32_t>> y;
    for (int j = 0; j < 2*r; ++j) {
        for (int i = 0; i < 16; i++)
        {
            y_moins_1[i] = y_moins_1[i] ^ B[j][i];
        }
            y_moins_1 = Salsa20_8(y_moins_1);
            y.push_back(y_moins_1);
    }
    for (int i = 0; i < r; ++i) {
        B[i] = y[2*i];
        B[r + i] = y[2*i + 1];
    }
    return B;
}

/**
 * ScryptROMix
 */
std::vector<std::vector<uint32_t>> ScryptROMix (std::vector<std::vector<uint32_t>> B, int r, uint32_t N) {
    std::vector<std::vector<uint32_t>> X(B);
    std::vector<std::vector<std::vector<uint32_t>>> V;
    for (uint32_t i = 0; i < N; ++i) {
        V.push_back(X);
        X = ScryptBlockMix(X, r);
    }

    int p;
    for (uint32_t i = 0; i < N; ++i) {
        p = X[2*r - 1][0] % N;
        for (int k = 0; k < 2*r; ++k)
        {
            for (int j = 0; j < 16; ++j)
            {
                X[k][j] ^= V[p][k][j];
            }
        }
        X = ScryptBlockMix(X, r);    
    }
    return X;
}

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

unsigned char *generate_salt() {
    unsigned char *salt;
    RAND_bytes(salt, 16);
    return salt;
}

uint8_t* scrypt(char *password, const unsigned char *salt, uint32_t N, int r, int p, int cLen) {
    unsigned char out[p*128*r];
    PKCS5_PBKDF2_HMAC(password, strlen(password), generate_salt(), 16, 1, EVP_sha256(), p*128*r, out);
    std::vector<std::vector<std::vector<uint32_t>>> B(buffer_to_triple_array(p, 2*r, 16, out));

    for (int i = 0; i < p; ++i) {
        B[i] = ScryptROMix(B[i], r, N);
    }

    unsigned char buffer[cLen];
    PKCS5_PBKDF2_HMAC(password, strlen(password), triple_array_to_buffer(B), p*128*r, 1, EVP_sha256(), cLen, buffer);

    return buffer;
}

char* pad (char *message) {
    int block_size = 16;
    int message_length = strlen(message);

    char padding = message_length % block_size;
    if (padding == 0) padding = block_size;

    char padded_message[message_length + padding];
    for (int i = 0; i < message_length; ++i) padded_message[i] = message[i];
    for (int i = 0; i < padding; ++i) padded_message[message_length + i] = padding;

    return padded_message;
}

char* unpad (char *padded_message) {
    int message_length = strlen(padded_message);
    char padding = padded_message[message_length - 1];

    char message[message_length - padding];
    for (int i = 0; i < message_length - padding; ++i) message[i] = padded_message[i];

    return message;
}