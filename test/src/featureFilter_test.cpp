#include <string>

#include <opencv2/opencv.hpp>

#include "Config.h"
#include "ORBDetector.h"
#include "ORBMatcher.h"

using namespace ORB_SLAM2_REWRITE;

int main(int argc, char **argv) {
    cv::Mat image1_show, image2_show;
    cv::Mat image1 =
        cv::imread("/media/rookie-lu/DATA1/Dataset/KITTI-ODOM/00/image_0/000000.png", cv::IMREAD_GRAYSCALE);
    cv::Mat image2 =
        cv::imread("/media/rookie-lu/DATA1/Dataset/KITTI-ODOM/00/image_0/000001.png", cv::IMREAD_GRAYSCALE);
    Config::Ptr config = Config::getInstance();
    ORBMatcher::Ptr matcher = ORBMatcher::getInstance();
    ORBDetector::Ptr detector = ORBDetector::getInstance();

    auto detector_cv = cv::ORB::create(1500);
    cv::BFMatcher matcher_cv(cv::NORM_HAMMING);

    std::vector<cv::KeyPoint> keyPoints1_cv, keyPoints2_cv;
    std::vector<Descriptor> desc1, desc2;
    cv::Mat desc1_cv, desc2_cv;

    detector_cv->detectAndCompute(image1, cv::noArray(), keyPoints1_cv, desc1_cv);
    detector_cv->detectAndCompute(image2, cv::noArray(), keyPoints2_cv, desc2_cv);

    std::vector<cv::DMatch> matchId_cv, goodMatches;
    matcher_cv.match(desc1_cv, desc2_cv, matchId_cv);
    matcher->buildHistogram(matchId_cv, keyPoints1_cv, keyPoints2_cv, 30, 3, goodMatches);

    cv::Mat match_cv, match_self;
    cv::drawMatches(image1, keyPoints1_cv, image2, keyPoints2_cv, goodMatches, match_cv);
    cv::drawMatches(image1, keyPoints1_cv, image2, keyPoints2_cv, matchId_cv, match_self);

    cv::imshow("FILTER", match_cv);
    cv::imshow("NO FILTER", match_self);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}