#include <iostream>
#include "datapak.hpp"

int main() {
    Datapak dat("bin/file.datapak");
    dat.purge();
}