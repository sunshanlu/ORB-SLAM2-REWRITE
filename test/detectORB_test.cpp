#include <vector>

#include <opencv2/opencv.hpp>

#include "Config.h"
#include "ORBDetector.h"

using namespace ORB_SLAM2_REWRITE;

int main(int argc, char **argv) {
    std::vector<cv::KeyPoint> keypoints_myself, keypoints_cv;
    auto orb = cv::ORB::create();
    cv::Mat image = cv::imread("/home/rookie-lu/Pictures/gaoda.jpeg", cv::IMREAD_GRAYSCALE);
    cv::Mat drawKp_myself, drawKp_cv;
    Config::Ptr config = Config::getInstance();
    ORBDetector detector;
    detector.detectORB(image, 500, Config::m_FAST_THRESHOLD_MAX, Config::m_FAST_THRESHOLD_MIN, keypoints_myself);
    orb->detect(image, keypoints_cv);
    cv::drawKeypoints(image, keypoints_myself, drawKp_myself);
    cv::drawKeypoints(image, keypoints_cv, drawKp_cv);
    cv::imshow("ORB_MYSELF", drawKp_myself);
    cv::imshow("ORB_CV", drawKp_cv);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}