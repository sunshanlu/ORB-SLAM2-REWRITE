#include <opencv2/opencv.hpp>

// cv::Mat::rowRange和colRange都是不会进行值的拷贝的，类似引用，从原图像中拿出一块
int main(int argc, char **argv) {
    cv::Mat img = cv::imread("/home/rookie-lu/Pictures/gaoda.jpeg", cv::IMREAD_GRAYSCALE);
    cv::Mat cell = img.rowRange(0, 200).colRange(0, 200);
    cell = cv::Mat::zeros(200, 200, CV_8UC1);
    cv::imshow("img", img);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}