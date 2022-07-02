#define ENABLE_DATAPAK_LOGGER
#include "datapak.hpp"
#include <iostream>

int main() {
    Datapak dat("bin/file.datapak");
    dat.purge();
    std::string family = "How How How How How How How How How How How How How How How How\n";
    dat.write("apple.txt", family);
    std::cout << dat.read("apple.txt") << std::endl;
}