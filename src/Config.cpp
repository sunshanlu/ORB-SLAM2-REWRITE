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
    m_FAST_THRESHOLD_MAX = root["FAST_THRESHOLD_MAX"].asInt();
    m_FAST_THRESHOLD_MIN = root["FAST_THRESHOLD_MIN"].asInt();
    m_BORDER_SIZE = root["BORDER_SIZE"].asInt();
    m_CELL_PATCH_SIZE = root["CELL_PATCH_SIZE"].asInt();
    m_MIN_CELL_FAST_NUM = root["MIN_CELL_FAST_NUM"].asInt();
    m_GRID_WIDTH = root["GRID_WIDTH"].asInt();
    m_GRID_HEIGHT = root["GRID_HEIGHT"].asInt();
    m_MATCH_HALF_WINDOW_SIZE = root["MATCH_HALF_WINDOW_SIZE"].asInt();
    m_MATCH_DISTANCE_RATIO = root["MATCH_DISTANCE_RATIO"].asFloat();
    m_MATCH_DISTANCE_THRESHOLD = root["MATCH_DISTANCE_THRESHOLD"].asInt();
    m_HISTOGRAM_BIN_SIZE = root["HISTOGRAM_BIN_SIZE"].asInt();
    m_HISTOGRAM_CHOOSE_NUM = root["HISTOGRAM_CHOOSE_NUM"].asInt();
}

int Config::m_GAUSS_BLUR_KERNEL_SIZE;
double Config::m_GAUSS_BLUR_SIGMA_X;
int Config::m_IWC_PATCH_SIZE;
int Config::m_HALF_IWC_PATCH_SIZE;
int Config::m_FAST_THRESHOLD_MAX;
int Config::m_FAST_THRESHOLD_MIN;
int Config::m_BORDER_SIZE;
int Config::m_CELL_PATCH_SIZE;
int Config::m_MIN_CELL_FAST_NUM;
int Config::m_GRID_WIDTH;
int Config::m_GRID_HEIGHT;
int Config::m_MATCH_HALF_WINDOW_SIZE;
float Config::m_MATCH_DISTANCE_RATIO;
int Config::m_MATCH_DISTANCE_THRESHOLD;
int Config::m_HISTOGRAM_BIN_SIZE;
int Config::m_HISTOGRAM_CHOOSE_NUM;

NAMESAPCE_END