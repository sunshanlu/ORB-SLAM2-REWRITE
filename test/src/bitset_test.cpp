#include <bitset>
#include <iostream>

int main() {
    std::bitset<256> set;
    set = ~set;
    std::cout << set << std::endl;
    return 0;
}