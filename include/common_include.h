#pragma once

#include <bitset>
#include <chrono>
#include <cmath>
#include <fstream>
#include <list>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <DBoW2/DBoW2.h>
#include <Eigen/Dense>
#include <json/json.h>
#include <opencv2/opencv.hpp>
#include <sophus/se3.hpp>
#include <spdlog/spdlog.h>

/// 命名空间开始宏
#define NAMESAPCE_BEGIN namespace ORB_SLAM2_REWRITE {

/// 命名空间结束宏
#define NAMESAPCE_END }

typedef std::bitset<256> Descriptor;                                      ///< 将biset<256>作为描述子
typedef std::vector<Descriptor> Descriptors;                              ///< 描述子的Vector集合
typedef DBoW2::TemplatedVocabulary<Descriptor, DBoW2::FBrief> Vocabulary; ///< 词袋模型
typedef DBoW2::FeatureVector FVector;                                     ///< DBoW2的特征向量
typedef std::vector<cv::DMatch> Matches;                                  ///< 匹配结果类型
typedef std::vector<cv::KeyPoint> KeyPoints;                              ///< 特征点类型