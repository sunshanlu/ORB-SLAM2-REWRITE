#include "ORBDetector.h"

using namespace ORB_SLAM2_REWRITE;

int main(int argc, const char **argv) {
    auto config = Config::getInstance();
    std::string fp(argv[1]);
    cv::Mat img = cv::imread(fp, cv::IMREAD_GRAYSCALE);
    auto imgVec = ORB_SLAM2_REWRITE::gaussianPyramid(img, 1.2, 8);
    int layerNum = 0;
    for (cv::Mat &image : imgVec) {
        cv::imshow(fmt::format("layer:{}", layerNum++), image);
    }
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}
