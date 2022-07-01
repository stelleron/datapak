#define ENABLE_DATAPAK_LOGGER
#include "datapak.hpp"
#include <iostream>

int main() {
    Datapak dat("bin/file.datapak");
    dat.purge();
    std::string family = "Occular Occular Occular Occular Potato Family Apply Rapply\n";
    dat.write("apple", family);
    std::cout << dat.read("apple") << std::endl;
}