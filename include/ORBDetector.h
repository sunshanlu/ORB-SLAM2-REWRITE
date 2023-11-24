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

    static Ptr getInstance() {
        static Ptr detector(new ORBDetector);
        return detector;
    }

    ORBDetector();

    // 检测FAST关键点，采用FAST_16_9
    void detectFAST(const cv::Mat &area, std::vector<FASTFeature::Ptr> &features, int threshold);

    bool detectFAST(const cv::Mat &area, const cv::Point &point, FASTFeature &feature, int threshold);

    void detectORB(const cv::Mat &image, int num, int maxThreshold, int minThreshold);

    // 单特征点灰度质心法
    void centroidMethod(const cv::Mat &image, cv::KeyPoint &fastPoint);

    // 多特征点灰度质心法
    void centroidMethod(const cv::Mat &image, std::vector<cv::KeyPoint> &fastPoints);

    // 单特征点计算描述子
    void computeDescriptor(const cv::Mat &image, const cv::KeyPoint &fastPoint, uchar *descriptor);

    // 多特征点计算描述子
    void computeDescriptor(const cv::Mat &image, const std::vector<cv::KeyPoint> &fastPoints, cv::Mat &descriptors);

private:
    static const int m_deltaXVec[16];                ///> FAST_16_9角点比较的X坐标
    static const int m_deltaYVec[16];                ///> FAST_16_9角点比较的Y坐标
    static const int m_speciId[4];                   ///> FAST_16_9角点的特殊索引（比满足三个以上）
    cv::Ptr<cv::FastFeatureDetector> m_fastDetector; ///> OpenCV的FAST特征点检测对象
    std::vector<int> m_deltaMaxU;                    ///> 不同v下，U的最大值，用于计算灰度质心
};

void pixelCirclePos(int halfPatchSize, std::vector<int> &deltaMaxU);

NAMESAPCE_END