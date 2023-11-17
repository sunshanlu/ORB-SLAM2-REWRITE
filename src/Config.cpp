#include "Config.h"

NAMESAPCE_BEGIN

Config::Config() {
    std::ifstream ifs("../config/config.json");
    std::string err;
    Json::Value root;
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;

    bool ret = Json::parseFromStream(builder, ifs, &root, &err);
    assert(ret == true);
    // TODO: 超参数
    m_PYRAMID_BORDER = root["PYRAMID_BORDER"].asInt();
    m_GAUSS_BLUR_SIGMA_X = root["GAUSS_BLUR_SIGMA_X"].asDouble();
}

int Config::m_PYRAMID_BORDER;
double Config::m_GAUSS_BLUR_SIGMA_X;

NAMESAPCE_END