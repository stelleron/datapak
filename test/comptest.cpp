#define SINFL_IMPLEMENTATION
#define SDEFL_IMPLEMENTATION
#include "sdefl.h"
#include "sinfl.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>

#define COMPRESSION_QUALITY_DEFLATE  8
#define FREE_DATA delete[]

template <typename T>
T* compressData(const T* data, int size, int* compSize) {
    struct sdefl sdefl = { 0 };
    int bounds = sdefl_bound(size);
    T* compData = new T[bounds];
    *compSize = sdeflate(&sdefl, compData, data, size, COMPRESSION_QUALITY_DEFLATE);
    std::cout << "SYSTEM: Compress data: Original size: " << size << " -> Comp. size: " << *compSize << std::endl;
    return compData;
}

template <typename T>
T* decompressData(const T* compData, int compSize, int* size) {
    T* data = new T[64*1024*1024];
    int length = sinflate(data, 64, compData, compSize);
    *size = length;
    std::cout << "SYSTEM: Decompress data: Comp. size: " << compSize << " -> Original size: " << *size << std::endl;
    return data;
}

int main() {
    int size;
    int compSize;
    std::string potato = "How How How How How How How How How How How How How How How How\n";

    char* apple = compressData<char>(potato.c_str(), potato.size(), &compSize);
    std::string potato

    FREE_DATA apple;
}