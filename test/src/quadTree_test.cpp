#include <opencv2/opencv.hpp>
#include <vector>

#include "QuadTree.h"

using namespace ORB_SLAM2_REWRITE;

int main(int argc, char **argv) {
    cv::Mat image = cv::imread("/home/rookie-lu/Pictures/gaoda.jpeg", cv::IMREAD_GRAYSCALE);
    cv::Mat drawNotQuad = image.clone();
    cv::Mat drawQuad = image.clone();
    std::vector<cv::KeyPoint> keypoints, quadKeypoints;
    cv::FAST(image, keypoints, 20, true);
    std::cout << keypoints.size() << std::endl;

    QuadTree quadTree(image, keypoints, 500);
    quadTree.getKeyPoints(quadKeypoints);

    cv::drawKeypoints(image, keypoints, drawNotQuad);
    cv::drawKeypoints(image, quadKeypoints, drawQuad);
    cv::imshow("drawNotQuad", drawNotQuad);
    cv::imshow("drawQuad", drawQuad);
    cv::waitKey(0);
    cv::destroyAllWindows();
}