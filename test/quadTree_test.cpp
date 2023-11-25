#include <opencv2/opencv.hpp>
#include <vector>

#include "QuadTree.h"

using namespace ORB_SLAM2_REWRITE;

int main(int argc, char **argv) {
    cv::Mat image = cv::imread("/home/rookie-lu/Pictures/gaoda.jpeg", cv::IMREAD_GRAYSCALE);
    std::vector<cv::KeyPoint> keypoints;
    cv::FAST(image, keypoints, 20, true);

    cv::drawKeypoints(image, keypoints, image);
    cv::imshow("image", image);
    cv::waitKey(0);
    cv::destroyAllWindows();
}