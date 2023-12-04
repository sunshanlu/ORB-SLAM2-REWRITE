#include <iostream>

#include <opencv2/opencv.hpp>

int main() {
    cv::Mat img = cv::imread("/home/rookie-lu/Pictures/gaoda.jpeg", cv::IMREAD_GRAYSCALE);
    cv::Mat dst1, dst2;
    cv::copyMakeBorder(img, dst1, 100, 100, 100, 100, cv::BORDER_REFLECT101);
    cv::copyMakeBorder(img, dst2, 100, 100, 100, 100, cv::BORDER_CONSTANT | cv::BORDER_REFLECT_101,
                       1); // 竟然和BORDER_REFLECT101效果相同

    // for (int i = 0; i < dst1.cols; ++i) {
    //     std::cout << (int)dst1.at<uchar>(99, i) << " " << (int)dst1.at<uchar>(101, i) << " ";
    // }
    // std::cout << std::endl;

    for (int i = 0; i < dst2.cols; ++i) {
        std::cout << (int)dst2.at<uchar>(99, i) << " ";
        std::cout << (int)dst1.at<uchar>(99, i) << " ";
        std::cout << (int)dst2.at<uchar>(101, i) << " ";
        std::cout << (int)dst1.at<uchar>(101, i) << " ";
    }
    std::cout << std::endl;
    return 0;
}