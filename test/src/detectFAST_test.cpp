#include "ORBDetector.h"

using namespace ORB_SLAM2_REWRITE;

const int m_deltaXVec[] = {0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -2, -3, -3, -3, -2, -1};
const int m_deltaYVec[] = {-3, -3, -2, -1, 0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -2, -3};

cv::Mat createImg() {
    cv::Mat img(7, 7, CV_8UC1);
    img.at<uchar>(3, 3) = 10;
    for (int i = 0; i < 16; ++i) {
        img.at<uchar>(3 + m_deltaYVec[i], 3 + m_deltaXVec[i]) = 2;
    }
    img.at<uchar>(0, 4) = 10;
    img.at<uchar>(6, 2) = 10;
    return std::move(img);
}

int main() {
    cv::Mat img = createImg();
    ORBDetector::Ptr detector = ORBDetector::getInstance();
    FASTFeature fast;
    bool ret = detector->detectFAST(img, cv::Point(3, 3), fast, 20);
    std::cout << ret << std::endl;
    std::cout << fast.m_position << std::endl;
    std::cout << fast.m_strength << std::endl;

    cv::Mat image = cv::imread("/home/rookie-lu/Pictures/gaoda.jpeg", cv::IMREAD_GRAYSCALE);
    cv::Mat imageD;
    cv::cvtColor(image, imageD, cv::COLOR_GRAY2BGR);
    cv::Mat imageCV = cv::imread("/home/rookie-lu/Pictures/gaoda.jpeg", cv::IMREAD_GRAYSCALE);
    std::vector<FASTFeature::Ptr> features;
    auto t = std::chrono::steady_clock::now();
    detector->detectFAST(image, features, 20);
    std::cout << "自定义FAST时间: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t).count()
              << std::endl;

    std::vector<cv::KeyPoint> ketPoints;
    auto fastCV = cv::FastFeatureDetector::create(20, false, cv::FastFeatureDetector::TYPE_9_16);
    fastCV->setNonmaxSuppression(true);

    t = std::chrono::steady_clock::now();
    fastCV->detect(imageCV, ketPoints);
    std::cout << "OpenCV的FAST时间: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t).count()
              << std::endl;

    cv::drawKeypoints(imageD, ketPoints, imageD, cv::Scalar(0, 0, 255));
    for (const auto feature : features) {
        cv::circle(imageD, feature->m_position, 3, cv::Scalar(255, 0, 0), -1);
    }

    cv::imshow("FAST TEST Compare", imageD);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}