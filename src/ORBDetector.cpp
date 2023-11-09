#include "ORBDetector.h"

NAMESAPCE_BEGIN

/**
 * @brief   获取图像的高斯金字塔
 * 根据指定图像和指定层数，获取高斯金字塔图像序列
 * @param   image[in] 给定图像
 * @param   layerNum[in] 给定金字塔层数
 * @return  std::vector<cv::Mat> 金字塔图像序列
 */
std::vector<cv::Mat> gaussianPyramid(const cv::Mat &image, uint layerNum) {
    assert(image.data != nullptr);
    std::vector<cv::Mat> imgSeries;
    imgSeries.reserve(layerNum);
    for (int i = 0; i < layerNum; ++i) {
        
    }

    return std::move(imgSeries);
}

NAMESAPCE_END