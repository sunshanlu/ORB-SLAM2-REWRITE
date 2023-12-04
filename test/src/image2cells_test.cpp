#include "Config.h"
#include "ORBDetector.h"

#include <opencv2/opencv.hpp>

using namespace ORB_SLAM2_REWRITE;

int main(int argc, char **argv) {
    Config::Ptr config = Config::getInstance();
    std::vector<ImageCell> cells;
    cv::Mat image = cv::imread("/home/rookie-lu/Pictures/gaoda.jpeg", cv::IMREAD_GRAYSCALE);
    image2Cells(image, cells);

    for (auto &cell : cells) {
        std::cout << cell.m_startPoint << std::endl;
    }
}
