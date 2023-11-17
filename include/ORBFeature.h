#pragma once
#include "common_include.h"

NAMESAPCE_BEGIN

class ORBFeature {};

/**
 * @brief FAST角点数据类型
 * 包括角点的位置和角点检测强度（采用连续满足阈值个数进行表达）
 */
struct FASTFeature {
    typedef std::shared_ptr<FASTFeature> Ptr;
    static Ptr createFeature() {
        Ptr feature(new FASTFeature);
        return feature;
    }
    cv::Point2f m_position; ///> FAST角点的位置
    int m_strength = 0;     ///> FAST角点的强度
};

NAMESAPCE_END