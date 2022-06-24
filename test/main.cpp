#include <iostream>
#include "snappy.h"
int main() {
    std::string input = "I mean business when I mean business!";
    std::string output;
    std::string output2;
    snappy::Compress(input.data(), input.size(), &output);
    std::cout << input.size() << std::endl;
    snappy::Uncompress(output.data(), output.size(), &output2);
    std::cout << output.size() << std::endl;
    std::cout << output2 << std::endl;
}