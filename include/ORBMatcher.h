#pragma once
#include "Config.h"
#include "common_include.h"

NAMESAPCE_BEGIN

struct Grid {
    typedef std::shared_ptr<Grid> Ptr;

    Grid(int minX, int maxX, int minY, int maxY, const std::vector<cv::KeyPoint> &keyPoints,
         const std::vector<Descriptor> &descriptors);

    static Ptr create(int minX, int maxX, int minY, int maxY, const std::vector<cv::KeyPoint> &keyPoints,
                      const std::vector<Descriptor> &descriptors) {
        Ptr grid(new Grid(minX, maxX, minY, maxY, keyPoints, descriptors));
        return grid;
    }

    static void initGrids(const cv::Mat &image, const std::vector<cv::KeyPoint> &keypoints,
                          const std::vector<Descriptor> descriptors, int gridWidth, int gridHeight,
                          std::vector<std::vector<Ptr>> &grids);

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
    int hammingDistance(const Descriptor &desc1, const Descriptor &desc2) {
        Descriptor compare = desc1 ^ desc2;
        return compare.count();
    }

    // 根据Hamming距离来寻找最优匹配
    bool getBMByHamming(const Descriptor &desc, const std::vector<Descriptor> &descs,
                        const std::vector<std::size_t> &candidateIDs, std::size_t &bestCanID, int &distance,
                        float ratio, int thresholdDistance);

    // 通过指定的第一张图片特征点，在第二张图片中寻找候选的特征点ID
    void getCandidateIDs(std::vector<std::size_t> &candidateIDs, const cv::Mat &image2, const cv::KeyPoint &kp1,
                         const std::vector<std::vector<Grid::Ptr>> &grids, const std::vector<cv::KeyPoint> &keyPoints2);
};

NAMESAPCE_END