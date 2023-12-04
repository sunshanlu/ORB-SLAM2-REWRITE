#include <iostream>

#include <opencv2/opencv.hpp>
// 虽然在opencv官网上有提到KeyPoint的angle的单位是度，但是经过测试，angle的赋值运算符并没有进行单位检测和转换
// 也就是说，keyPoint中的单位是什么，你自己知道就可以，opencv并不做任何多余的单位转换
int main(int argc, char **argv) {
    cv::KeyPoint kpt;
    kpt.angle = -100000;
    std::cout << kpt.angle << std::endl;
    return 0;
}