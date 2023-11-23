#include "Config.h"

NAMESAPCE_BEGIN

const std::string CONFIG_FILE_PATH = "/home/rookie-lu/Project/ORB-SLAM/ORB-SLAM2-REWRITE/config/config.json";

Config::Config() {
    std::ifstream ifs(CONFIG_FILE_PATH);
    std::string err;
    Json::Value root;
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;

    bool ret = Json::parseFromStream(builder, ifs, &root, &err);
    assert(ret == true);

    m_GAUSS_BLUR_KERNEL_SIZE = root["GAUSS_BLUR_KERNEL_SIZE"].asInt();
    m_GAUSS_BLUR_SIGMA_X = root["GAUSS_BLUR_SIGMA_X"].asDouble();
    m_IWC_PATCH_SIZE = root["IWC_PATCH_SIZE"].asInt();
    assert(m_IWC_PATCH_SIZE % 2 == 1);
    m_HALF_IWC_PATCH_SIZE = (m_IWC_PATCH_SIZE - 1) / 2;
}

int Config::m_GAUSS_BLUR_KERNEL_SIZE;
double Config::m_GAUSS_BLUR_SIGMA_X;
int Config::m_IWC_PATCH_SIZE;
int Config::m_HALF_IWC_PATCH_SIZE;

NAMESAPCE_END