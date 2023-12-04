#include <opencv2/opencv.hpp>

uchar data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
cv::Mat arr2Mat() {

    cv::Mat mats(2, 5, CV_8UC1, data);
    return mats;
}

int main(int argc, char **argv) {
    cv::Mat mat;
    mat = std::move(arr2Mat());
    std::cout << mat << std::endl;
    return 0;
}