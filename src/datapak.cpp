#include <sys/stat.h>
#include <iostream>
#include <string.h>
#include "snappy-stubs-public.h"
#include "snappy.h"
#include "datapak.hpp"

#define COMP_ARGS(input, output) input.data(), input.size(), &output
#define COMPRESS(input, output) snappy::Compress(COMP_ARGS(input, output))
#define DECOMPRESS(input, output) snappy::Uncompress(COMP_ARGS(input, output))

// Impl. for datapak
Datapak::Datapak(const char* filename) {
    //== First check if the file exists
    struct stat buffer;
    bool fileExists = (stat(filename, &buffer) == 0);
    if (!fileExists) {
        std::cout << "File does not exist. Creating new .datapak" << std::endl;
        file = fopen(filename, "w");
        fclose(file);
    }

    //== Then check if the file has been initialised. If not, write a new header to it
    file = fopen(filename, "a+");
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
    fclose(file);

    //== Now open the file for reading only
    file = fopen(filename, "r");
    //== Then read the the file header
    fseek(file, 0, SEEK_SET);
    fread(&header, sizeof(header), 1, file);
    //== Check if versions are compatiable as well
    if (strcmp(header.version, DATAPAK_VERSION) != 0) {
        std::cout << "File version is not compatiable with Datapak!" << std::endl;
        exit(0);
    }
    //== Now read all of the data chunks
    chunks.resize(header.dataCount);
    std::cout << "Found " << header.dataCount << " chunks. Loading them..." << std::endl;
    char* cache; // Used to cache string data to be written to the chunk array
    for(int x = 0; x < header.dataCount; x++) {
        fread(&chunks[x].header, sizeof(chunks[x].header), 1, file);
        cache = new char[chunks[x].header.baseSize];
        fread(cache, chunks[x].header.baseSize, 1, file);
        chunks[x].data = cache;
        delete[] cache;
        std::cout << chunks[x].header.alias << std::endl;
        std::cout << chunks[x].data << std::endl;
    }
    //== Finally close the file and save the filename for later
    std::cout << "Loaded all chunks!\n";
    fclose(file);
    this->filename = filename;
}

Datapak::~Datapak() {
    //== Open the file for write operations
    file = fopen(filename.c_str(), "w");
    //== Now serialize the file
    fseek(file, 0, SEEK_SET);
    fwrite(&header, sizeof(header), 1, file);
        
    for(int x = 0; x < header.dataCount; x++) {
        fwrite(&chunks[x].header, sizeof(chunks[x].header), 1, file);
        fwrite(chunks[x].data.c_str(), chunks[x].header.compSize, 1, file);
    }
    //== Finally close the file
    fclose(file);
}   

void Datapak::write(const char* alias, const std::string& data) {
    // Create a new chunk
    DataChunk chunk;
    strcpy(chunk.header.alias, alias);
    chunk.header.baseSize = data.size(); 
    COMPRESS(data, chunk.data);
    chunk.header.compSize = chunk.data.size();
    // Then add it to the chunk array
    chunks.push_back(chunk);
    header.dataCount += 1;
}