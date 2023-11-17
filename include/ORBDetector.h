#pragma once
#include "CircleID.h"
#include "Config.h"
#include "ORBFeature.h"
#include "common_include.h"
NAMESAPCE_BEGIN

// 高斯金字塔
std::vector<cv::Mat> gaussianPyramid(const cv::Mat &image, double scale, int layerNum);

class ORBDetector {
public:
    typedef std::shared_ptr<ORBDetector> Ptr;

    // 检测FAST关键点，采用FAST_16_9
    void detectFAST(const cv::Mat &area, std::vector<FASTFeature::Ptr> &features, int threshold);

    bool detectFAST(const cv::Mat &area, const cv::Point &point, FASTFeature &feature, int threshold);

private:
    static const int m_deltaXVec[16]; ///> FAST_16_9角点比较的X坐标
    static const int m_deltaYVec[16]; ///> FAST_16_9角点比较的Y坐标
    static const int m_speciId[4];    ///> FAST_16_9角点的特殊索引（比满足三个以上）
};

NAMESAPCE_END