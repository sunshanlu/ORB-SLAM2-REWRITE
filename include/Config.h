#pragma once

#include "common_include.h"

NAMESAPCE_BEGIN

class Config {
public:
    typedef std::shared_ptr<Config> Ptr;
    Config();
    static Ptr getInstance() {
        static Ptr config(new Config);
        return config;
    }

public:
    static int m_GAUSS_BLUR_KERNEL_SIZE;   ///< 高斯模糊核大小
    static double m_GAUSS_BLUR_SIGMA_X;    ///< 高斯模糊的标准差
    static int m_IWC_PATCH_SIZE;           ///< 定义灰度质心法计算旋转角所需要的直径大小
    static int m_HALF_IWC_PATCH_SIZE;      ///< 灰度质心法半径
    static int m_FAST_THRESHOLD_MAX;       ///< FAST检测的阈值最大值
    static int m_FAST_THRESHOLD_MIN;       ///< FAST检测的阈值最小值（检测角点较少时使用该阈值）
    static int m_BORDER_SIZE;              ///< 图像边界大小
    static int m_CELL_PATCH_SIZE;          ///< 图像块大小（实际会比这个值大6，原因在于FAST半径）
    static int m_MIN_CELL_FAST_NUM;        ///< 图像块
    static int m_GRID_WIDTH;               ///< 关键点匹配的Grid宽度信息
    static int m_GRID_HEIGHT;              ///< 关键点匹配的Grid高度信息
    static int m_MATCH_HALF_WINDOW_SIZE;   ///< 关键点匹配的窗口半径
    static int m_MATCH_DISTANCE_THRESHOLD; ///< 关键点匹配的Hamming距离阈值
    static float m_MATCH_DISTANCE_RATIO;   ///< 关键点匹配的Hamming距离比值
    static int m_HISTOGRAM_BIN_SIZE;       ///< 直方图的bin大小（用于特征匹配筛选）
    static int m_HISTOGRAM_CHOOSE_NUM;     ///< 直方图选择个数（用于特征匹配筛选）
};

NAMESAPCE_END