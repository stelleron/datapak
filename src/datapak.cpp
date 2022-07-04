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
#define COMP_QUALITY 8
#define MAX_DECOMP_SIZE 64 * 1024 * 1024 // in MB
#define FREE_DATA(arg) delete[] arg
#define FIND_ERROR LOG("Unable to find data under the given alias!")

// Compression functions
template <typename T>
T* compressData(const T* data, int size, int* compSize) {
    struct sdefl sdefl = { 0 };
    int bounds = sdefl_bound(size);
    T* compData = new T[bounds];
    *compSize = sdeflate(&sdefl, compData, data, size, COMP_QUALITY);
    return compData;
}

template <typename T>
T* decompressData(const T* compData, int compSize, int* size) {
    T* data = new T[MAX_DECOMP_SIZE];
    int length = sinflate(data, MAX_DECOMP_SIZE, compData, compSize);
    T* buffer = (T*)realloc(data, length);

    if (buffer != NULL) data = buffer;
    else LOG("SYSTEM: Failed to re-allocate required decompression memory");

    *size = length;
    return data;
}

// Impl. for datapak
Datapak::Datapak() {
    isLoaded = false;
}

Datapak::Datapak(const char* filename) {
    load(filename);
}

void Datapak::load(const char* filename) {
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

    for(int x = 0; x < header.dataCount; x++) {
        // First read the header
        fread(&chunks[x].header, sizeof(chunks[x].header), 1, file);
        // Then the data
        chunks[x].data = new char[chunks[x].header.baseSize];
        fread(chunks[x].data, chunks[x].header.baseSize, 1, file);
        // NOTE: Remember to free all aloocated data
    }
    //== Finally close the file and save the filename for later
    LOG("Loaded all chunks!");
    fclose(file);
    this->filename = filename;
    isClosed = false;
    isLoaded = true;
}

Datapak::~Datapak() {
    close();
}   

bool Datapak::find(const char* alias) {
    // Find the alias
    for (int x = 0; x < header.dataCount; x++) {
        if (strcmp(alias, chunks[x].header.alias) == 0) {
            // Set the pointer to the data chunk
            ptr = x;
            return true;
        }
    }
    return false;
}

int Datapak::getBaseSize(const char* alias) {
    // Find the chunk then get the size
    if (find(alias)) {
        return chunks[ptr].header.baseSize;
    }
    else {
        FIND_ERROR;
        return 0;
    }
}

int Datapak::getCompSize(const char* alias) {
    // Find the chunk then get the size
    if (find(alias)) {
        return chunks[ptr].header.compSize;
    }
    else {
        FIND_ERROR;
        return 0;
    }
}

void Datapak::write(const char* alias, const std::string& data) {
    // First check if the alias exists
    if (find(alias)) {
        // If it does, rewrite the stored data
        FREE_DATA(chunks[ptr].data);
        chunks[ptr].header.baseSize = data.size();
        chunks[ptr].data = compressData<char>(data.data(), data.size(), &chunks[ptr].header.compSize);
    }
    else {
        // Else add a new chunk
        DataChunk chunk;
        strcpy(chunk.header.alias, alias);

        chunk.header.baseSize = data.size();
        chunk.data = compressData<char>(data.data(), data.size(), &chunk.header.compSize);

        chunks.push_back(chunk);
        header.dataCount += 1;   
    }
}

std::string Datapak::read(const char* alias) {
    // First check if the alias exists
    if (find(alias)) {
        char* decompData = decompressData<char>(chunks[ptr].data, chunks[ptr].header.compSize, &chunks[ptr].header.baseSize);
        std::string nData = decompData;
        FREE_DATA(decompData);
        return nData;
    }
    else {
        FIND_ERROR;
        return 0;
    }    
}

void Datapak::close() {
    if (!isClosed && isLoaded) { // Check if the file is closed first
        //== Open the file for write operations
        file = fopen(filename.c_str(), "w");
        //== Now serialize the file
        fseek(file, 0, SEEK_SET);
        fwrite(&header, sizeof(header), 1, file);
            
        for(int x = 0; x < header.dataCount; x++) {
            fwrite(&chunks[x].header, sizeof(chunks[x].header), 1, file);
            fwrite(chunks[x].data, chunks[x].header.compSize, 1, file);
        }
        //== Finally close the file
        fclose(file);
        LOG("Closed the datapak!");
        isLoaded = false;
    }
}

void Datapak::purge() {
    // Reset all stored data
    header.dataCount = 0;
    chunks.clear();
}

// Iterate through the chunks and find the one to be removed
void Datapak::remove(const char* alias) {
    // Iterate through the chunks and find the one to be removed
    for (auto iter = chunks.begin(); iter != chunks.end(); ++iter) {
        if (strcmp(iter->header.alias, alias) == 0) {
            chunks.erase(iter);
            header.dataCount -= 1;
            return;
        }
    }
    FIND_ERROR;
}

void Datapak::list() {
    for (auto iter = chunks.begin(); iter != chunks.end(); ++iter) {
        LOG(iter->header.alias);
    }
}

void Datapak::rename(const char* alias, const char* new_alias) {
    if(find(alias)) {
        strcpy(chunks[ptr].header.alias, new_alias);
    }
    else {
        FIND_ERROR;
    }
}