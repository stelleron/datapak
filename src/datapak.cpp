#include <sys/stat.h>
#include <iostream>
#include "snappy.h"
#include "datapak.hpp"

#define COMP_ARGS(input, output) input.data(), input.size(), &output
#define COMPRESS(input, output) snappy::Compress(COMP_ARGS(input, output))
#define DECOMPRESS(input, output) snappy::Uncompress(COMP_ARGS(input, output))

// Impl. for datapak
Datapak::Datapak(const char* filename) {
    //== First check if the file exists == 
    struct stat buffer;
    bool fileExists = (stat(filename, &buffer) == 0);
    if (!fileExists) {
        std::cout << "File does not exist. Creating new .datapak" << std::endl;
        file.open(filename, std::ios::out); 
        file.close();
    }
    file.open(filename, std::ios::binary);
    //== Then read the file to get the file header ==
    // Move read pointer to beginning of the file
    file.seekg(0, std::ios_base::beg);
    file.read((char*)&header, sizeof(header));
}