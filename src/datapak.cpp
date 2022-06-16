#include <sys/stat.h>
#include <iostream>
#include "datapak.hpp"

DataPak::DataPak(const char* filename, const char accessType) {
    // First check if the file exists
    struct stat buffer;
    bool fileExists = (stat(filename, &buffer) == 0);
    if (fileExists) {
        std::cout << "File exists! Loading the .datapak..." << std::endl;
        // If the file isn't open in write mode, first iterate over the lines in the file and search for all aliases
        file.open(filename, std::ios::in);
        std::string line;
        // Iterate over lines
        while (std::getline(file, line)) {
            
        }
        file.close();
    }
    else {
        std::cout << "File does not exist. Creating new .datapak" << std::endl;
        file.open(filename, std::ios::out); 
        file.close();
    }

    // Decide open type based on access type
    if (accessType == 'r') {
        file.open(filename, std::ios::in); 
    }
    else if (accessType == 'w') {
        file.open(filename, std::ios::out); 
    }
    else if (accessType == 'a') {
        file.open(filename, std::ios_base::app);
    }
    else {
        std::cout << "Unspecified file access operation! Use \'r\' to read a .datapak or \'w\' to write to a .datapak\n";
    }
    // Save data to the datapak class
    this->filename = filename;
    this->accessType = accessType;
}

DataPak::~DataPak() {
    file.close();
}

void DataPak::writeData(const char* alias, unsigned char* dataptr, int size) {
    // Ensure the file isn't opened in read mode
    if (accessType == 'r') {
        std::cout << "File opened for reading and hence cannot be modified!\n";
        return;
    }

    // First add the alias 
    file << "[" << alias << "]" << "\n";
    // Then add the file data
    for (int x = 0; x < size; ++x) {
        file << dataptr[x];
    }
    // Add a newline to the end
    file << "\n";
}