#include "ORBMatcher.h"

NAMESAPCE_BEGIN

/**
 * @brief Grid的唯一构造函数
 * 这里进行Grid构造的时候，将在边界上的特征点都进行计入
 *
 * @param minX      Grid的最小X
 * @param maxX      Grid的最大X
 * @param minY      Grid的最小Y
 * @param maxY      Grid的最大Y
 * @param keyPoints 图像的所有关键点信息
 */
Grid::Grid(int minX, int maxX, int minY, int maxY, const std::vector<cv::KeyPoint> &keyPoints,
           const std::vector<Descriptor> &descriptors)
    : m_minX(minX)
    , m_minY(minY)
    , m_maxX(maxX)
    , m_maxY(maxY) {
    for (int idx = 0; idx < keyPoints.size(); ++idx) {
        auto &kp = keyPoints[idx];
        if (kp.pt.x <= maxX && kp.pt.x >= minX && kp.pt.y <= maxY && kp.pt.y >= minY) {
            m_ids.push_back(idx);
            ++m_pointsNum;
        }
    }
}

/**
 * @brief 根据图片和规定的Grid宽高信息，进行Grids的构建
 *
 * @param image         输入图片信息
 * @param keypoints     图片中的特征点信息
 * @param gridWidth     规定的Grid宽度信息
 * @param gridHeight    规定的Grid高度信息
 * @param grids         输出的Grid集合信息
 */
void Grid::initGrids(const cv::Mat &image, const std::vector<cv::KeyPoint> &keypoints,
                     const std::vector<Descriptor> descriptors, int gridWidth, int gridHeight,
                     std::vector<std::vector<Grid::Ptr>> &grids) {
    std::vector<int> rows, cols;
    for (int row = 0; row < image.rows; row += gridHeight)
        rows.push_back(row);
    for (int col = 0; col < image.cols; col += gridWidth)
        cols.push_back(col);
    rows.push_back(image.rows);
    cols.push_back(image.cols);
    for (int i = 0; i < rows.size() - 1; ++i) {
        std::vector<Ptr> rowGrids;
        for (int j = 0; j < cols.size() - 1; ++j) {
            auto grid = Grid::create(cols[j], cols[j + 1], rows[i], rows[i + 1], keypoints, descriptors);
            rowGrids.push_back(grid);
        }
        grids.push_back(rowGrids);
    }
}

/**
 * @brief 两张位姿相近的图片之间寻找特征匹配
 *
 * @param image2        被寻找特征匹配的图片
 * @param keyPoints1    图片1中的关键点信息
 * @param keyPoints2    图片2中的关键点信息
 * @param desc1         图片1中的描述子信息
 * @param desc2         图片2中的描述子信息
 * @param matchIDs      输出的匹配id信息cv::DMatch类型
 */
void ORBMatcher::matchFeatureToInit(const cv::Mat &image2, const std::vector<cv::KeyPoint> &keyPoints1,
                                    const std::vector<cv::KeyPoint> &keyPoints2, const std::vector<Descriptor> &desc1,
                                    const std::vector<Descriptor> &desc2, std::vector<cv::DMatch> &matchIDs) {
    std::vector<std::vector<Grid::Ptr>> grids;
    std::vector<cv::DMatch> matchCanIDs;
    Grid::initGrids(image2, keyPoints2, desc2, Config::m_GRID_WIDTH, Config::m_GRID_HEIGHT, grids);

    for (int idx = 0; idx < keyPoints1.size(); ++idx) {
        std::vector<std::size_t> candidatePointIds;
        const auto &kp1 = keyPoints1[idx];
        const auto &desc = desc1[idx];
        getCandidateIDs(candidatePointIds, image2, kp1, grids, keyPoints2);
        if (candidatePointIds.size() < 2) {
            continue;
        }
        std::size_t bestCanID;
        int bestDistance = 0;
        if (getBMByHamming(desc, desc2, candidatePointIds, bestCanID, bestDistance, Config::m_MATCH_DISTANCE_RATIO,
                           Config::m_MATCH_DISTANCE_THRESHOLD))
            matchCanIDs.emplace_back(idx, bestCanID, 0, bestDistance);
    }
    buildHistogram(matchCanIDs, keyPoints1, keyPoints2, Config::m_HISTOGRAM_BIN_SIZE, Config::m_HISTOGRAM_CHOOSE_NUM,
                   matchIDs);
}

/**
 * @brief 通过指定的第一张图片特征点，在第二张图片中寻找候选的特征点ID
 * 这里需要采用set进行去重，原因是Grid中将边界上的特征点也进行了统计
 * @param candidateIDs  输出的候选点ID容器
 * @param image2        输入图片2
 * @param kp1           输入指定的图片1中的特征点
 * @param grids         输入初始化好的图片2的Grid集合
 * @param keyPoints2    图片2中的所有关键点信息
 */
void ORBMatcher::getCandidateIDs(std::vector<std::size_t> &candidateIDs, const cv::Mat &image2, const cv::KeyPoint &kp1,
                                 const std::vector<std::vector<Grid::Ptr>> &grids,
                                 const std::vector<cv::KeyPoint> &keyPoints2) {
    assert(candidateIDs.empty() && "断言候选ID容器为空");
    std::set<std::size_t> candidateIDs_set; // 由于Grid统计的关键点包含边界上的点，因此可能存在重复ID
    int gridID[4];
    int cellXmin = std::max(0.f, kp1.pt.x - Config::m_MATCH_HALF_WINDOW_SIZE);
    int cellXmax = std::min((float)image2.cols, kp1.pt.x + Config::m_MATCH_HALF_WINDOW_SIZE);
    int cellYmin = std::max(0.f, kp1.pt.y - Config::m_MATCH_HALF_WINDOW_SIZE);
    int cellYmax = std::min((float)image2.rows, kp1.pt.y + Config::m_MATCH_HALF_WINDOW_SIZE);
    getGridID(gridID, grids, cellXmax, cellXmin, cellYmax, cellYmin);
    const int &rowBegin = gridID[0];
    const int &rowEnd = gridID[1];
    const int &colBegin = gridID[2];
    const int &colEnd = gridID[3];

    for (int row = rowBegin; row <= rowEnd; ++row) {
        auto &rowGrids = grids[row];
        for (int col = colBegin; col <= colEnd; ++col) {
            Grid::Ptr grid = rowGrids[col];
            if (grid->m_pointsNum == 0) {
                continue;
            }
            /// @note 对grid中，在圆内的点进行拷贝，并利用set进行去重处理
            for (auto &id : grid->m_ids) {
                if (std::pow((keyPoints2[id].pt.x - kp1.pt.x), 2) + std::pow((keyPoints2[id].pt.y - kp1.pt.y), 2) <=
                    std::pow(Config::m_MATCH_HALF_WINDOW_SIZE, 2)) {
                    candidateIDs_set.insert(id);
                }
            }
        }
    }
    // 最后将set去重的信息拷贝到候选ID的vector中即可
    std::copy(candidateIDs_set.begin(), candidateIDs_set.end(), std::back_inserter(candidateIDs));
}

/**
 * @brief 根据边界信息，获取涉及到的Grid的ID
 *
 * @param gridID    输出的索引信息，包括行开始，行终止，列开始，列终止
 * @param grids     输入的Grids容器信息
 * @param cellXmax  区域的最大X值
 * @param cellXmin  区域的最小X值
 * @param cellYmax  区域的最大Y值
 * @param cellYmin  区域的最小Y值
 */
void ORBMatcher::getGridID(int *gridID, const std::vector<std::vector<Grid::Ptr>> &grids, int cellXmax, int cellXmin,
                           int cellYmax, int cellYmin) {
    int rowBegin = -1, rowEnd = -1, colBegin = -1, colEnd = -1;
    for (int rowID = 0; rowID < grids.size(); ++rowID) {
        Grid::Ptr rowStart = grids[rowID][0];
        if (rowStart->m_maxY >= cellYmin && rowStart->m_minY <= cellYmin)
            rowBegin = rowID;
        if (rowStart->m_maxY >= cellYmax && rowStart->m_minY <= cellYmax) {
            rowEnd = rowID;
            break;
        }
    }
    for (int colID = 0; colID < grids[0].size(); ++colID) {
        Grid::Ptr colStart = grids[0][colID];
        if (colStart->m_maxX >= cellXmin && colStart->m_minX <= cellXmin)
            colBegin = colID;
        if (colStart->m_maxX >= cellXmax && colStart->m_minX <= cellXmax) {
            colEnd = colID;
            break;
        }
    }
    assert(rowBegin >= 0 && rowEnd >= 0 && colBegin >= 0 && colEnd >= 0 && "索引不合法");
    gridID[0] = rowBegin;
    gridID[1] = rowEnd;
    gridID[2] = colBegin;
    gridID[3] = colEnd;
}

/**
 * @brief 根据Hamming距离来寻找最优匹配
 * 最优候选描述子应该具备如下两个条件：
 *      1. 最优Hamming/次最优Hamming距离的比值小于ratio
 *      2. 最优的Hamming距离应该小于thresholdDistance
 * @param desc              输入的被匹配的描述子信息
 * @param descs             输入的匹配的描述子信息集合
 * @param candidateIDs      输入的候选描述子对应的索引
 * @param bestCanID         输出最优的候选描述子索引
 * @param ratio             最优/次最优距离的比值要求
 * @param thresholdDistance 最优匹配对应的Hamming距离的阈值要求
 * @return true             表示找到了符合条件的最优候选描述子
 * @return false            未找到符合条件的候选描述子
 */
bool ORBMatcher::getBMByHamming(const Descriptor &desc, const std::vector<Descriptor> &descs,
                                const std::vector<std::size_t> &candidateIDs, std::size_t &bestCanID, int &outDistance,
                                float ratio, int thresholdDistance) {
    std::size_t bestID, secondBestID;
    int bestDistance, secondBestDistance;

    // 对bestID，secondBestID，bestDistance和secondBestDistance进行初始化
    int distance1 = hammingDistance(desc, descs[candidateIDs[0]]);
    int distance2 = hammingDistance(desc, descs[candidateIDs[1]]);
    if (distance1 < distance2) {
        bestID = 0;
        secondBestID = 1;
        bestDistance = distance1;
        secondBestDistance = distance2;
    } else {
        bestID = 1;
        secondBestID = 0;
        bestDistance = distance2;
        secondBestDistance = distance1;
    }

    for (std::size_t idx = 2; idx < candidateIDs.size(); ++idx) {
        int distance = hammingDistance(desc, descs[candidateIDs[idx]]);
        if (distance < bestDistance) {
            bestID = idx;
            bestDistance = distance;
        } else if (distance < secondBestDistance) {
            secondBestID = idx;
            secondBestDistance = distance;
        }
    }
    bestCanID = bestID;
    outDistance = bestDistance;
    if ((float)bestDistance / secondBestDistance < ratio && bestDistance < thresholdDistance) {
        return true;
    }
    return false;
}

void ORBMatcher::buildHistogram(const std::vector<cv::DMatch> &matches, const std::vector<cv::KeyPoint> &keypoints1,
                                const std::vector<cv::KeyPoint> &keypoints2, const int binNum, const int chooseNum,
                                std::vector<cv::DMatch> &goodMatches) {
    assert(chooseNum > 0 && binNum > chooseNum && binNum <= 360 && "参数不合法");
    static std::vector<std::vector<const cv::DMatch *>> histogram(binNum);
    const static float angleStep = 360.f / binNum;

    for (const auto &match : matches) {
        float diffAngle = keypoints1[match.queryIdx].angle - keypoints2[match.trainIdx].angle;
        diffAngle = diffAngle >= 0 ? diffAngle : diffAngle + 360.f; // diffAngle 在区间[0, 360)
        histogram[(int)diffAngle / angleStep].push_back(&match);
    }

    // 采用部分冒泡排序算法，将含有最多匹配点的bin放到前面
    partBubble(histogram, binNum, chooseNum);
    for (int i = 0; i < chooseNum; ++i) {
        for (auto &match : histogram[i]) {
            goodMatches.push_back(*match);
        }
    }

    // 将histogram清空
    for (auto &bin : histogram) {
        bin.clear();
    }
}
NAMESAPCE_END
