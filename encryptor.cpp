#include <iostream>
#include "encryptor.hpp"
#include "utils.hpp"
#include "core.hpp"

int main(int argc, char const *argv[])
{
   if (argc != 5) {
    std::cout << "Usage: ./encryptor enc/dec pwd input output" << std::endl;
       return EXIT_FAILURE;
   } else if (argv[1] == "enc") {
       std::cout << "TODO: encryption" << std::endl;
   } else {
        std::cout << "TODO: decryption" << std::endl;
   }
    return 0;
}
