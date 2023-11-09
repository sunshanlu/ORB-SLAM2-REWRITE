#pragma once
#include "common_include.h"
NAMESAPCE_BEGIN

std::vector<cv::Mat> gaussianPyramid(const cv::Mat &image, uint layerNum);

NAMESAPCE_END