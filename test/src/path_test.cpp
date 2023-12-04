#include "common_include.h"
#include <fstream>
#include <iostream>

int main(int argc, const char **argv) {
    std::ifstream ifs("../config/config.json");
    std::string str;
    ifs >> str;
    std::cout << str << std::endl;
    return 0;
}