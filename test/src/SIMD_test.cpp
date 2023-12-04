#include "CircleID.h"
#include "ORBDetector.h"
#include "ORBFeature.h"
#include "common_include.h"

#include <immintrin.h>

using namespace ORB_SLAM2_REWRITE;
typedef __m128i uchar16;

const int m_deltaXVec[] = {0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -2, -3, -3, -3, -2, -1};
const int m_deltaYVec[] = {-3, -3, -2, -1, 0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -2, -3};
const int m_speciId[] = {0, 4, 8, 12};
bool detectFAST(const cv::Mat &area, const cv::Point &point, FASTFeature &feature, uchar threshold) {
    int row = point.y, col = point.x;
    uchar centerPixel = area.at<uchar>(row, col);
    uchar arrRoundPixel[16];
    uchar diffPixel[16];
    for (int i = 0; i < 16; ++i)
        arrRoundPixel[i] = area.at<uchar>(row + m_deltaYVec[i], col + m_deltaXVec[i]);

    // 为了保证精度，在向量相减之前，调换大小位置
    uchar16 centerPixelM = _mm_set1_epi8(centerPixel);
    uchar16 roundPixelM = _mm_loadu_si128((const uchar16 *)arrRoundPixel);
    __m128i mask = _mm_cmpgt_epi8(centerPixelM, roundPixelM);
    __m128i tmp = centerPixelM;
    centerPixelM = _mm_blendv_epi8(roundPixelM, centerPixelM, mask);
    roundPixelM = _mm_blendv_epi8(tmp, roundPixelM, mask);
    centerPixelM = _mm_sub_epi8(centerPixelM, roundPixelM);
    mask = _mm_cmpgt_epi8(centerPixelM, _mm_set1_epi8(threshold)); // todo:这里可能会有精度损失，如果说差值大于127

    // 找到第一个差值小于阈值的数，对应的id
    int idx;
    int firstValue = _mm_extract_epi8(centerPixelM, 0);
    if (firstValue < threshold && firstValue > 0) {
        idx = 0;
    } else {
        __m128i zero = _mm_setzero_si128();
        __m128i cmpRet = _mm_cmpeq_epi8(mask, zero);
        int initSucc = _mm_movemask_epi8(cmpRet);
        if (!initSucc) { // 初始化失败的情况
            feature.m_position.x = point.x;
            feature.m_position.y = point.y;
            feature.m_strength = 16;
            return true;
        }
        idx = _tzcnt_u32(initSucc);
    }

    // idx往后的位置前移，0位开始的位置拼接
    int params[16];
    int startID = 0, creatID = 0;
    for (int i = idx; i < 16 - idx - 1; ++i)
        params[startID++] = i;
    for (; startID < 16; ++startID)
        params[startID] = creatID++;
    uchar16 shuffleMask = _mm_loadu_si128((uchar16 *)params);
    mask = _mm_shuffle_epi8(mask, shuffleMask);
    _mm_storeu_si128((uchar16 *)diffPixel, mask);

    // todo:判断4个特殊位置是否
    int matchNum = 0, matchNum2 = 0;
    for (int id = 0; id < 16; ++id) {
        if (diffPixel[id]) {
            ++matchNum2;
        } else {
            matchNum = matchNum > matchNum2 ? matchNum : matchNum2;
            matchNum2 = 0;
        }
    }
    matchNum = matchNum > matchNum2 ? matchNum : matchNum2;
    if (matchNum >= 9) {
        feature.m_position.x = point.x;
        feature.m_position.y = point.y;
        feature.m_strength = matchNum;
        return true;
    } else {
        return false;
    }
}

cv::Mat createImg() {
    cv::Mat img(7, 7, CV_8UC1, cv::Scalar(0));
    img.at<uchar>(3, 3) = 10;
    for (int i = 0; i < 16; ++i) {
        img.at<uchar>(3 + m_deltaYVec[i], 3 + m_deltaXVec[i]) = 2;
        img.at<uchar>(3 + m_deltaYVec[i], 4 + m_deltaXVec[i]) = 2;
    }
    // img.at<uchar>(0, 4) = 10;
    // img.at<uchar>(6, 2) = 10;

    return std::move(img);
}

int main() {
    cv::Mat img = createImg();
    cv::Point point(3, 3);
    FASTFeature feature;
    ORBDetector::Ptr detect = ORBDetector::getInstance();

    auto t_point = std::chrono::steady_clock::now();
    detect->detectFAST(img, point, feature, 5);
    std::cout << (std::chrono::steady_clock::now() - t_point).count() << std::endl;

    t_point = std::chrono::steady_clock::now();
    bool ret = detectFAST(img, point, feature, 5);
    std::cout << (std::chrono::steady_clock::now() - t_point).count() << std::endl;
    std::cout << feature.m_strength << std::endl;
    std::cout << feature.m_position << std::endl;

    return 0;
}