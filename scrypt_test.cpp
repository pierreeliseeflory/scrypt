#include <iostream>
#include <cstdint>
#include <vector>
#include "scrypt_test.hpp"
#include "scrypt.hpp"

bool TestSalsa20_8 () {
    uint32_t test_array[16] = {0x219a877e, 0x86c93e4f, 0xe640a97c, 0x268f7141, 0x5b55eeba, 0xb5c1618c, 0x1146f80d, 0x1d3bcd6d, 0x19f324ee, 0x853d9bdf, 0x4b1e1214, 0x32aac55a, 0x291d0276, 0x2948c709, 0x8dc6ebed, 0x5ec2b8b8};
    uint32_t test_array_output[16] = {0x9c851fa4, 0x99cc0866, 0xcbca813b, 0x5ef0c02, 0x81214b04, 0x7d33fda2, 0x631c7bfd, 0x292f6896, 0x683139b4, 0xbce6c9e3, 0xb7c56bfe, 0xba966da0, 0x10cc24e4, 0x5c74912c, 0x3d67ad24, 0x818f61c7};
    std::vector<uint32_t> test_vector(test_array, test_array + 16);
    test_vector = Salsa20_8(test_vector);
    for (int i =0; i < 16; ++i) {
        if (test_vector[i] != test_array_output[i]) return false;
    }
    return true;
}

bool TestScryptBlockMix () {
    int r = 1;
    uint32_t test_array[2*r][16] = {{0x650bcef7, 0xa4722d3d, 0xabf58c10, 0xddff12e9, 0xdb167677, 0xea727bb, 0xaef30482, 0xad6f0f2d, 0x488ff689, 0x7be8d111, 0x40d73bcc, 0x29fd9f0a, 0x84014f09, 0xf3749563, 0x31a1e59a, 0xd7bc1752},
                                    {0x44914989, 0x22bb1372, 0x4db5256c, 0xfb7063a8, 0x804398cd, 0xbb664637, 0xbfb5fc8f, 0xb054c240, 0x517cd267, 0xfed54ace, 0xbc929d8, 0x1b575a50, 0xad1c4d7f, 0xda3c526a, 0xbc670e77, 0x897eafea}};
    uint32_t test_array_output[2*r][16] = {{0x9c851fa4, 0x99cc0866, 0xcbca813b, 0x5ef0c02, 0x81214b04, 0x7d33fda2, 0x631c7bfd, 0x292f6896, 0x683139b4, 0xbce6c9e3, 0xb7c56bfe, 0xba966da0, 0x10cc24e4, 0x5c74912c, 0x3d67ad24, 0x818f61c7},
                                            {0x75c9ed20, 0xa8813832, 0x4cf64005, 0x3ccd2d16, 0xfe7c0721, 0xe25f8d5f, 0x8f16a4b1, 0xb7783695, 0x803d3b7d, 0xabe4603b, 0xe5960992, 0xb6534d9b, 0x58222a5d, 0xf5edd577, 0xf1b92c84, 0x25e4ef4e}};

    std::vector<std::vector<uint32_t>> test_vector;
    std::vector<uint32_t> part1(test_array[0], test_array[0] + 16);
    std::vector<uint32_t> part2(test_array[1], test_array[1] + 16);
    test_vector.push_back(part1);
    test_vector.push_back(part2);

    test_vector = ScryptBlockMix(test_vector, r);
    for (int j = 0; j < r; ++j) {
        for (int i =0; i < 16; ++i) {
            if (test_vector[j][i] != test_array_output[j][i]) {
                std::cout << std::hex << test_vector[j][i] << " " << std::hex << test_array_output[j][i] << std::endl;
                return false;
            }
        }
    }
    return true;
}

/**
 * TestScryptROMix
 * 
 * vecteurs de tests venant de: https://www.ietf.org/rfc/rfc7914.html#page-11
 */
bool TestScryptROMix () {
    int r = 1;
    uint32_t N = 16;
    uint32_t test_array[2*r][16] = {{0x650bcef7, 0xa4722d3d, 0xabf58c10, 0xddff12e9, 0xdb167677, 0xea727bb, 0xaef30482, 0xad6f0f2d, 0x488ff689, 0x7be8d111, 0x40d73bcc, 0x29fd9f0a, 0x84014f09, 0xf3749563, 0x31a1e59a, 0xd7bc1752},
                                    {0x44914989, 0x22bb1372, 0x4db5256c, 0xfb7063a8, 0x804398cd, 0xbb664637, 0xbfb5fc8f, 0xb054c240, 0x517cd267, 0xfed54ace, 0xbc929d8, 0x1b575a50, 0xad1c4d7f, 0xda3c526a, 0xbc670e77, 0x897eafea}};
    uint32_t test_array_output[2*r][16] = {{0x93c1cc79, 0xcaeb9d62, 0x700b7f04, 0xb6f64b60, 0x4adde32c, 0x55e32696, 0x9861fcfa, 0x462beae6, 0x671384d5, 0x29b0993b, 0x57c365d6, 0x26b41f60, 0xbbf4b2a0, 0x9fee00a2, 0x9bd1430a, 0x719c1a57},
                                            {0xe64211ef, 0x6f265a5d, 0x2c83cadd, 0x7caa9fe5, 0xf19c0bac, 0xcaff2bbe, 0xee010d30, 0xc4197638, 0x44fd12ae, 0xa003f238, 0x7ec4e1e4, 0x1f8614c3, 0xcb87904e, 0x686a3933, 0xd2f9e873, 0x8e4b9a53}};
    std::vector<std::vector<uint32_t>> test_vector;
    std::vector<uint32_t> part1(test_array[0], test_array[0] + 16);
    std::vector<uint32_t> part2(test_array[1], test_array[1] + 16);
    test_vector.push_back(part1);
    test_vector.push_back(part2);

    std::vector<std::vector<uint32_t>> test_vector_output = ScryptROMix(test_vector, r, N);
    for (int j = 0; j < r; ++j) {
        for (int i =0; i < 16; ++i) {
            if (test_vector_output[j][i] != test_array_output[j][i]) {
                std::cout << std::hex << test_vector_output[j][i] << " " << std::hex << test_array_output[j][i] << std::endl;
                return false;
            }
        }
    }
    return true;
}

bool RunTests() {
    std::cout << "Test des fonctions:" << std::endl;
    bool all_tests_correct = true;
    if (TestSalsa20_8()) std::cout << " [-] Salsa20/8 correct." << std::endl;
    else {
        std::cout << " [X] Salsa20/8 incorrect." << std::endl;
        all_tests_correct = false;
    }
    if (TestScryptBlockMix()) std::cout << " [-] ScryptBlockMix correct." << std::endl;
    else {
        std::cout << " [X] ScryptBlockMix incorrect." << std::endl;
        all_tests_correct = false;
    }
    if (TestScryptROMix()) std::cout << " [-] ScryptROMix correct." << std::endl;
    else {
        std::cout << " [X] ScryptROMix incorrect." << std::endl;
        all_tests_correct = false;
    }
    return all_tests_correct;
}

int main(int argc, char const *argv[])
{
    if (!RunTests()) return EXIT_FAILURE;

    std::cout << "==============================" << std::endl;
    std::cout << "Tous les tests ont réussi" << std::endl;
    return EXIT_SUCCESS;
}