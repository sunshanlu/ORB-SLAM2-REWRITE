//
// Created by rookie-lu on 23-11-29.
//
#include <opencv2/opencv.hpp>

// 注意，FAST函数内部会对keypoints进行清除操作，然后再进行FAST特征点检测

int main()
{
    cv::Mat image = cv::imread("/home/rookie-lu/Pictures/gaoda.jpeg", cv::IMREAD_GRAYSCALE);
    std::vector<cv::KeyPoint> keyPoints;
    keyPoints.push_back(cv::KeyPoint());
    cv::FAST(image, keyPoints, 20, true);
    std::cout << keyPoints.size() << std::endl;
    cv::FAST(image, keyPoints, 20, true);
    std::cout << keyPoints.size() << std::endl;
    return 0;
}