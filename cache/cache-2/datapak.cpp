#include <sys/stat.h>
#include <string.h>
#include <iostream>
#define SINFL_IMPLEMENTATION
#define SDEFL_IMPLEMENTATION
#include "sdefl.h"
#include "sinfl.h"
#include "datapak.hpp"
#include <stdlib.h>

#define LOG(arg) std::cout << arg << std::endl
#define COMP_QUALITY  8
#define MAX_DECOMP_SIZE 32
#define FREE_DATA delete[]

// Compression functions
template <typename T>
T* compressData(const T* data, int size, int* compSize) {
    struct sdefl sdefl = { 0 };
    int bounds = sdefl_bound(size);
    T* compData = new T[bounds];
    *compSize = sdeflate(&sdefl, compData, data, size, COMP_QUALITY);
    std::cout << "SYSTEM: Compress data: Original size: " << size << " -> Comp. size: " << *compSize << std::endl;
    return compData;
}

template <typename T>
T* decompressData(const T* compData, int compSize, int* size) {
    T* data = new T[MAX_DECOMP_SIZE*1024*1024];
    int length = sinflate(data, MAX_DECOMP_SIZE, compData, compSize);
    T* temp = (T*)realloc(data, length);
    if (temp != NULL) data = temp;
    else std::cout << "SYSTEM: Failed to re-allocate required decompression memory" << std::endl;
    *size = length;
    std::cout << "SYSTEM: Decompress data: Original size: " << *size << " <- Comp. size: " << compSize << std::endl;
    return data;
}

// Impl. for datapak
Datapak::Datapak(const char* filename) {
    //== First check if the file exists
    struct stat buffer;
    bool fileExists = (stat(filename, &buffer) == 0);
    if (!fileExists) {
        LOG("File does not exist. Creating new .datapak");
        file = fopen(filename, "w");
        fclose(file);
    }

    //== Then check if the file has been initialised. If not, write a new header to it
    file = fopen(filename, "a+");
    fseek(file, 0, SEEK_END);
    {
        int size = ftell(file);
        if (size == 0) {
            LOG("File is empty. Initialising file...");
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
        LOG("File version is not compatiable with Datapak!");
        exit(0);
    }
    //== Now read all of the data chunks
    chunks.resize(header.dataCount);
    LOG("Found " << header.dataCount << " chunks. Loading them...");
    char* cache; // Used to cache string data to be written to the chunk array
    for(int x = 0; x < header.dataCount; x++) {
        fread(&chunks[x].header, sizeof(chunks[x].header), 1, file);
        cache = new char[chunks[x].header.baseSize];
        fread(cache, chunks[x].header.baseSize, 1, file);
        chunks[x].data = cache;
        delete[] cache;
    }
    //== Finally close the file and save the filename for later
    LOG("Loaded all chunks!");
    fclose(file);
    this->filename = filename;
    isClosed = false;
}

Datapak::~Datapak() {
    close();
}   

void Datapak::write(const char* alias, const std::string& data) {
    // If the alias already exists, rewrite the existing data
    if (find(alias)) {
        LOG("Rewriting existing data stored in the given alias: " << alias);
        for (int x = 0; x < header.dataCount; x++) {
            if (strcmp(alias, chunks[x].header.alias) == 0) {
                chunks[x].header.baseSize = data.size();
                char* compData = compressData<char>(data.c_str(), data.size(), &chunks[x].header.compSize);
                chunks[x].data = compData;
                FREE_DATA compData;
                return;
            }
        }
    }
    // Else add the data to the chunk array
    else {
        // Create a new chunk
        DataChunk chunk;
        strcpy(chunk.header.alias, alias);

        chunk.header.baseSize = data.size();
        char* compData = compressData<char>(data.c_str(), data.size(), &chunk.header.compSize);
        chunk.data = compData;
        FREE_DATA compData;
        // Add the chunk to the chunk array
        chunks.push_back(chunk);
        header.dataCount += 1;   
        return;
    }
}

std::string Datapak::read(const char* alias) {
    // First find the header in the chunk array
    for (int x = 0; x < header.dataCount; x++) {
        if (strcmp(alias, chunks[x].header.alias) == 0) {
            char* decompData = decompressData<char>(chunks[x].data.c_str(), chunks[x].header.compSize, &chunks[x].header.baseSize);
            std::string nData = decompData;
            FREE_DATA decompData;
            // Then read it to the string
            return nData;
        }
    }
    LOG("Unable to find data under the given alias!");
    return "";
}

void Datapak::close() {
    if (!isClosed) { // Check if the file is closed first
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
}

void Datapak::purge() {
    // Reset all stored data
    header.dataCount = 0;
    chunks.clear();
}

bool Datapak::find(const char* alias) {
    // Find the alias
    for (int x = 0; x < header.dataCount; x++) {
        if (strcmp(alias, chunks[x].header.alias) == 0) {
            return true;
        }
    }
    return false;
}

int Datapak::getBaseSize(const char* alias) {
    for (int x = 0; x < header.dataCount; x++) {
        if (strcmp(alias, chunks[x].header.alias) == 0) {
            return chunks[x].header.baseSize;
        }
    }
    LOG("Unable to find data under the given alias!");
    return 0;
}

int Datapak::getCompSize(const char* alias) {
    for (int x = 0; x < header.dataCount; x++) {
        if (strcmp(alias, chunks[x].header.alias) == 0) {
            return chunks[x].header.compSize;
        }
    }
    LOG("Unable to find data under the given alias!");
    return 0;
}

void Datapak::remove(const char* alias) {
    // Iterate through the chunks and find the one to be removed
    for (auto iter = chunks.begin(); iter != chunks.end(); ++iter) {
        if (strcmp(iter->header.alias, alias) == 0) {
            chunks.erase(iter);
            header.dataCount -= 1;
            return;
        }
    }
    LOG("Unable to find data under the given alias!");
}