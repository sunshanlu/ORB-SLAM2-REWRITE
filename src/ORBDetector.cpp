#include "ORBDetector.h"

NAMESAPCE_BEGIN

/**
 * @brief   获取图像的高斯金字塔
 * 根据指定图像和指定层数，获取高斯金字塔图像序列
 * @param   image[in] 给定图像
 * @param   layerNum[in] 给定金字塔层数
 * @return  std::vector<cv::Mat> 金字塔图像序列
 */
std::vector<cv::Mat> gaussianPyramid(const cv::Mat &image, double scale, int layerNum) {
    assert(image.data != nullptr);
    std::vector<cv::Mat> imgSeries;
    imgSeries.reserve(layerNum);
    cv::Mat scaledImg = image;
    for (int i = 0; i < layerNum; ++i) {
        if (i == 0) {
            imgSeries.push_back(scaledImg);
            continue;
        }
        cv::Mat temp;
        cv::Size newSize(std::round(scaledImg.cols / scale), std::round(scaledImg.rows / scale));
        cv::GaussianBlur(scaledImg, temp, cv::Size(Config::m_PYRAMID_BORDER, Config::m_PYRAMID_BORDER),
                         Config::m_GAUSS_BLUR_SIGMA_X);
        cv::resize(temp, temp, newSize);
        imgSeries.push_back(temp);
        scaledImg = std::move(temp);
    }
    return std::move(imgSeries);
}

void ORBDetector::detectFAST(const cv::Mat &area, std::vector<FASTFeature::Ptr> &features, int threshold = 20) {
    for (int row = 3; row < area.rows - 3; ++row) {
        for (int col = 3; col < area.cols - 3; ++col) {
            FASTFeature::Ptr feature = FASTFeature::createFeature();
            bool ret = detectFAST(area, cv::Point(col, row), *feature, threshold);
            if (ret)
                features.push_back(feature);
        }
    }
}

bool ORBDetector::detectFAST(const cv::Mat &area, const cv::Point &point, FASTFeature &feature, int threshold) {
    int row = point.y, col = point.x;
    int pointVal = (int)area.at<uchar>(row, col);
    // double thresholdVal = pointVal * threshold / 100.0; // 阈值为原来像素的 threshold%
    double thresholdVal = threshold;
    std::vector<int> specMatchId;
    specMatchId.reserve(4);
    int outThNum = 0, sepcOutThNum = 0;
    for (int idx = 0; idx < 4; ++idx) {
        // 特殊id的灰度值
        int pointValSpe = (int)area.at<uchar>(row + m_deltaYVec[m_speciId[idx]], col + m_deltaXVec[m_speciId[idx]]);
        if (std::abs(pointValSpe - pointVal) > thresholdVal) {
            ++sepcOutThNum;
            specMatchId.push_back(m_speciId[idx]);
        }
    }
    if (sepcOutThNum < 3)
        return false;
    std::vector<bool> matchId;
    matchId.reserve(16);
    CircleID circleID(16);
    bool setBeginID = false;
    for (int i = 0; i < 16; ++i) {
        int pixelValue = (int)area.at<uchar>(row + m_deltaYVec[i], col + m_deltaXVec[i]);
        if (std::abs(pixelValue - pointVal) > thresholdVal)
            matchId.push_back(true);
        else {
            matchId.push_back(false);
            if (!setBeginID) {
                circleID.setValue(i);
                setBeginID = true;
            }
        }
    }
    // 判断初始化失败的情况（全为true）
    if (setBeginID == false) {
        feature.m_position.x = point.x;
        feature.m_position.y = point.y;
        feature.m_strength = 16;
        return true;
    }
    int matchNum = 0, matchNum2 = 0;
    CircleID id = circleID;
    ++id;
    for (; id != circleID; ++id) {
        if (matchId[id.getValue()]) {
            ++matchNum2;
        } else {
            matchNum = matchNum > matchNum2 ? matchNum : matchNum2;
            matchNum2 = 0;
        }
    }
    matchNum = matchNum > matchNum2 ? matchNum : matchNum2;
    matchNum2 = 0;
    if (matchNum >= 9) {
        feature.m_position.x = point.x;
        feature.m_position.y = point.y;
        feature.m_strength = matchNum;
        return true;
    } else {
        return false;
    }
}

const int ORBDetector::m_deltaXVec[] = {0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -2, -3, -3, -3, -2, -1};
const int ORBDetector::m_deltaYVec[] = {-3, -3, -2, -1, 0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -2, -3};
const int ORBDetector::m_speciId[] = {0, 4, 8, 12};

NAMESAPCE_END