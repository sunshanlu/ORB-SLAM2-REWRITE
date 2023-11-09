#pragma once

#include "common_include.h"

NAMESAPCE_BEGIN

class Config {
public:
    Config(const std::string &fp);

public:
    int m_PYRAMID_BORDER; ///> 金字塔图像高斯模糊边界

private:
};

NAMESAPCE_END