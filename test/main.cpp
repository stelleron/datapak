#define ENABLE_DATAPAK_LOGGER
#include "datapak.hpp"
#include <iostream>

int main() {
    Datapak dat("bin/file.datapak");
    std::cout << dat.read("apple.txt");
}