#pragma once

#include "Config.h"
#include "common_include.h"

NAMESAPCE_BEGIN

struct Grid {
    typedef std::shared_ptr<Grid> Ptr;

    Grid(int minX, int maxX, int minY, int maxY, const std::vector<cv::KeyPoint> &keyPoints);

    static Ptr create(int minX, int maxX, int minY, int maxY, const std::vector<cv::KeyPoint> &keyPoints) {
        Ptr grid(new Grid(minX, maxX, minY, maxY, keyPoints));
        return grid;
    }

    static void initGrids(const cv::Mat &image, const std::vector<cv::KeyPoint> &keypoints, int gridWidth,
                          int gridHeight, std::vector<std::vector<Ptr>> &grids);

    int m_minX, m_maxX, m_minY, m_maxY; ///< Grid的范围限定元素
    std::vector<std::size_t> m_ids;     ///< Grid范围内对应的特征点ID和描述子ID
    int m_pointsNum = 0;                ///< Grid范围内的特征点个数
};

class ORBMatcher {
public:
    typedef std::shared_ptr<ORBMatcher> Ptr;

    // 删除拷贝构造函数
    ORBMatcher(const ORBMatcher &other) = delete;

    // 删除拷贝赋值运算符
    ORBMatcher &operator=(const ORBMatcher &other) = delete;

    static Ptr getInstance() {
        static Ptr matcher(new ORBMatcher);
        return matcher;
    }

    // 两张位姿相近的图片之间寻找特征匹配
    void matchFeatureToInit(const cv::Mat &image2, const std::vector<cv::KeyPoint> &keyPoints1,
                            const std::vector<cv::KeyPoint> &keyPoints2, const std::vector<Descriptor> &desc1,
                            const std::vector<Descriptor> &desc2, std::vector<cv::DMatch> &matchIDs);

    // 通过词袋模型进行匹配
    void matchFeatureByBoW(Vocabulary &vocab, DBoW2::FeatureVector &features1, FVector &features2, Matches &matches,
                           KeyPoints &keypoints1, KeyPoints &keypoints2, Descriptors &desc1, Descriptors &desc2);

    // 构建直方图
    void buildHistogram(const std::vector<cv::DMatch> &matches, const std::vector<cv::KeyPoint> &keypoints1,
                        const std::vector<cv::KeyPoint> &keypoints2, const int &binNum, const int &chooseNum,
                        std::vector<cv::DMatch> &goodMatches);

private:
    ORBMatcher() = default; ///< 单例设计模式

    // 根据边界信息，获取涉及到的Grid的ID
    void getGridID(int *gridID, const std::vector<std::vector<Grid::Ptr>> &grids, int cellXmax, int cellXmin,
                   int cellYmax, int cellYmin);

    /**
     * @brief 比较两个描述子之间的hamming距离
     *
     * @param desc1 输入描述子1
     * @param desc2 输入描述子2
     * @return int  输出两个描述子之间的hamming距离
     */
    static int hammingDistance(const Descriptor &desc1, const Descriptor &desc2) {
        int distance = (int)(desc1 ^ desc2).count();
        return distance;
    }

    // 根据Hamming距离来寻找最优匹配
    bool getBMByHamming(const Descriptor &desc, const std::vector<const Descriptor*> descs,
                        const std::vector<unsigned> &candidateIDs, std::size_t &bestCanID, int &distance,
                        float ratio, int thresholdDistance);

    // 通过指定的第一张图片特征点，在第二张图片中寻找候选的特征点ID
    void getCandidateIDs(std::vector<unsigned> &candidateIDs, const cv::Mat &image2, const cv::KeyPoint &kp1,
                         const std::vector<std::vector<Grid::Ptr>> &grids, const std::vector<cv::KeyPoint> &keyPoints2);

    /**
     * @brief 部分冒牌排序，将前chooseNum个含有最多元素的bin排在前面
     *
     * @param histogram 输入的直方图
     * @param binNum    直方图的长度
     * @param chooseNum 指定要排序的bin的个数
     */
    template <typename T> void partBubble(std::vector<std::vector<T>> &histogram, int binNum, int chooseNum) {
        for (int step = 0; step < chooseNum; ++step) {
            for (int binID = binNum - 1; binID > step; --binID) {
                if (histogram[binID].size() > histogram[binID - 1].size()) {
                    std::swap(histogram[binID], histogram[binID - 1]);
                }
            }
        }
    }
};

NAMESAPCE_END