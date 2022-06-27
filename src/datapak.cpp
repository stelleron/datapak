#include <sys/stat.h>
#include <iostream>
#include <string.h>
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
        file = fopen(filename, "w");
        fclose(file);
    }
    //== Then load the file for read/append operations
    file = fopen(filename, "a+");
    //== Then check if the file has been initialised. If not, write a new header to it
    fseek(file, 0, SEEK_END);
    {
        int size = ftell(file);
        if (size == 0) {
            std::cout << "File is empty. Initialising file..." << std::endl;
            FileHeader nHeader;
                strcpy(nHeader.version, DATAPAK_VERSION);
                nHeader.dataCount = 0;
                fwrite(&nHeader, sizeof(nHeader), 1, file);
        }
    }
}

Datapak::~Datapak() {
    fclose(file);
}   