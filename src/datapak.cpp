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

}

Datapak::~Datapak() {
    
}