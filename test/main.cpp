#include "datapak.hpp"
#include <iostream>

int main() {
    // Open a datapak file or create a new datapak and store it here
    Datapak dat;
    dat.load("bin/file.datapak"); // Load the file
    dat.purge(); // Erase all data from the datapak

    std::string helloworld = "HelloWorld HelloWorld HelloWorld HelloWorld HelloWorld HelloWorld \n";
    dat.write("hello.txt", helloworld); // Write data to the datapak under a given alias used to reference the file

    std::cout << dat.read("hello.txt"); // Load the data stored under the given alias
    std::cout << dat.find("helo.txt") << std::endl; // Check whether data exists under the given header(returns FALSE here)

    dat.rename("hello.txt", "apple.txt"); // Rename a file
    dat.list(); // List all file names in the given directory

    dat.remove("hello.txt"); // Remove a file from the given header
    dat.close(); // Close the file and serialize all stored data(automatically called when Datapak leaves scope)
}
