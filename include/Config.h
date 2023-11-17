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
    static int m_PYRAMID_BORDER;
    static double m_GAUSS_BLUR_SIGMA_X;
};

NAMESAPCE_END