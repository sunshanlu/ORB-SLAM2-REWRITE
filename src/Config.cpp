#include "Config.h"

NAMESAPCE_BEGIN

Config::Config(const std::string &fp) {
    std::ifstream ifs(fp);
    std::string err;
    Json::Value root;
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;

    bool ret = Json::parseFromStream(builder, ifs, &root, &err);
    assert(ret == true);
    m_PYRAMID_BORDER = root["PYRAMID_BORDER"].asInt();
}

NAMESAPCE_END