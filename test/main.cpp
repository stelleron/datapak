#include <iostream>
#include "datapak.hpp"

int main() {
    Datapak dat("bin/file.datapak");
    dat.purge(); 
    std::string family = "apple\n";
    dat.write("chess.txt", family);
    std::cout << dat.getSize("chess.txt") << std::endl;
}