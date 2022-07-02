#define ENABLE_DATAPAK_LOGGER // Enable the logger to output messages
#include "datapak.hpp"
#include <iostream>

int main() {
    // Open a datapak file or create a new datapak and store it here
    Datapak dat("bin/file.datapak");
    dat.purge(); // Erase all data from the datapak

    std::string helloworld = "sjdwjfodsjfdisfofsijosdjfodshttps://github.com/microsoft/vscode/issues/36069";
    dat.write("hello.txt", helloworld); // Write data to the datapak under a given alias used to reference the file

    std::cout << dat.read("hello.txt") << std::endl; // Load the data stored under the given alias
}