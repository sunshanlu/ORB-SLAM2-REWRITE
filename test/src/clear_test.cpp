#include <iostream>
#include <vector>

// clear()函数会将vector中的所有元素都清空，并不是更改元素
int main() {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::cout << v.size() << std::endl;
    v.clear();
    std::cout << v.size() << std::endl;

    return 0;
}