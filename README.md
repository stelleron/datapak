# Datapak -  A lightweight C++ packaging library

## Summary
Datapak is a C++ library that is designed with simplicity in mind to package and compress files into a single .datapak file (file extension can be renamed). Datapak uses the DEFLATE algorithm to compress data and requires only the `datapak.hpp` and `datapak.cpp` files along with the DEFLATE headers.

## Setup
Datapak is included with a premake5.lua file to build it, as well as the Makefiles. If you wish to make the project, just simply install it from GitHub, and have GNU make install. Then, use the terminal to open the project and simply just type `make`.

For using the premake5.lua version, download Premake from [here](https://premake.github.io/) and follow setup instructions. Then, use Premake to generate whatever project type you wish using `premake5 (enter your build type here)` and build the project.

You can also simply include the headers in `external/` as well as `datapak.cpp` and `datapak.hpp` into your project.

## Version
Currently the version of Datapak is 1.0.0. If the Datapak file has a different version than the Datapak library it will not be able to read the file.

### Example
```cpp
#define ENABLE_DATAPAK_LOGGER // Enable the logger to output messages
#include "datapak.hpp"
#include <iostream>

int main() {
    // Open a datapak file or create a new datapak and store it here
    Datapak dat("bin/file.datapak");
    dat.purge(); // Erase all data from the datapak

    std::string helloworld = "HelloWorld HelloWorld HelloWorld HelloWorld HelloWorld HelloWorld\n";
    dat.write("hello.txt", helloworld); // Write data to the datapak under a given alias used to reference the file

    std::cout << dat.read("hello.txt"); // Load the data stored under the given alias
    std::cout << dat.find("helo.txt"); // Check whether data exists under the given header(returns FALSE here)

    dat.remove("hello.txt"); // Remove a file from the given header
    dat.close(); // Close the file and serialize all stored data(automatically called when Datapak leaves scope)
}
```

## License
Datapak is open-source and uses the MIT license. Check LICENSE to read more about the terms of usage.

## Acknowledgements
Thanks to `https://github.com/vurtun/lib` for providing the small DEFLATE libraries.