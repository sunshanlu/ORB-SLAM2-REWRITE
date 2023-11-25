#include "QuadTree.h"

NAMESAPCE_BEGIN
/**
 * @brief 分裂子节点使用的有参构造函数
 *
 * @param minX      最小x坐标
 * @param maxX      最大x坐标
 * @param minY      最小y坐标
 * @param maxY      最大y坐标
 * @param parent    父节点
 */
QuadTreeNode::QuadTreeNode(float minX, float maxX, float minY, float maxY, QuadTreeNode *parent)
    : m_parent(parent)
    , m_minX(minX)
    , m_maxX(maxX)
    , m_minY(minY)
    , m_maxY(maxY) {
    assert(m_parent && "assert parent is not null");
    for (const auto &keyPoint : m_parent->m_keyPoints) {
        ///@note 采取边界剔除策略，不计入在边界上的特征点
        if (keyPoint->pt.x < m_maxX && keyPoint->pt.x > m_minX && keyPoint->pt.y > m_minY && keyPoint->pt.y < m_maxY) {
            m_keyPoints.insert(keyPoint);
        }
    }
}

/**
 * @brief 根节点使用的有参构造函数
 *
 * @param minX      最小x坐标
 * @param maxX      最大x坐标
 * @param minY      最小y坐标
 * @param maxY      最大y坐标
 * @param keypoints 关键点集合
 */
QuadTreeNode::QuadTreeNode(float minX, float maxX, float minY, float maxY, const std::vector<cv::KeyPoint> &keypoints)
    : m_minX(minX)
    , m_maxX(maxX)
    , m_minY(minY)
    , m_maxY(maxY) {
    for (const auto &keyPoint : keypoints) {
        m_keyPoints.insert(&keyPoint);
    }
}

/**
 * @brief QuadTree构造函数
 * 不断的对图像块进行四叉树分解，区域内关键点个数多的首先参加分解，这时的分解顺序是由m_nodes的数据类型set保证的
 * @param image     图像
 * @param keypoints 关键点
 * @param num       要求节点数
 */
QuadTree::QuadTree(const cv::Mat &image, const std::vector<cv::KeyPoint> &keypoints, int num)
    : m_reqNum(num) {
    m_root = new QuadTreeNode(0, image.cols, 0, image.rows, keypoints);
    split(m_root);
    for (auto &node : m_root->m_children) {
        if (node->m_keyPoints.size() > 0) {
            m_nodes.insert(node);
            ++m_num;
        }
    }
    int lastNum = m_num;
    std::set<QuadTreeNode *, PointsNumCompare> tmpNodes;
    std::copy(m_nodes.begin(), m_nodes.end(), std::inserter(tmpNodes, tmpNodes.begin()));
    while (m_num < num) {
        for (auto &node : tmpNodes) {
            if (node->m_keyPoints.size() == 1 || m_num >= num) {
                break;
            }
            split(node);
            m_nodes.erase(node); // 注意，这里需要将node从m_nodes中删除，保证m_nodes的意义
            for (auto &nodeChi : node->m_children) {
                if (nodeChi->m_keyPoints.size() > 0) {
                    m_nodes.insert(nodeChi);
                    ++m_num;
                }
            }
        }
        if (lastNum == m_num)
            break;
        lastNum = m_num;
        tmpNodes.clear();
        std::copy(m_nodes.begin(), m_nodes.end(), std::inserter(tmpNodes, tmpNodes.begin()));
    }
    if (m_num < num)
        m_matchCond = false;
}

/**
 * @brief 给定父节点，分裂子节点
 * 根据minX,minY,maxX,maxY四个点的中点，将父节点分裂成四个子节点
 * @param parent 父节点
 */
void QuadTree::split(QuadTreeNode *parent) {
    float medianX = parent->m_minX + (parent->m_maxX - parent->m_minX) / 2;
    float medianY = parent->m_minY + (parent->m_maxY - parent->m_minY) / 2;
    float arrX[8] = {parent->m_minX, medianX, medianX, parent->m_maxX,
                     parent->m_minX, medianX, medianX, parent->m_maxX};
    float arrY[8] = {parent->m_minY, medianY,        parent->m_minY, medianY,
                     medianY,        parent->m_maxY, medianY,        parent->m_maxY};
    for (int i = 0; i < 4; i++) {
        QuadTreeNode *child = new QuadTreeNode(arrX[2 * i], arrX[2 * i + 1], arrY[2 * i], arrY[2 * i + 1], parent);
        child->m_parent = parent;
        parent->m_children[i] = child;
    }
}

/**
 * @brief 获取每个区域节点中，响应最大的节点（并不是所有节点放在一起后，进行重排序，提升部分效率）
 * 这种做法，是要比所有关键点放在一起进行重排序优秀的多，原因在于，哪些面积大，并且关键点只有1个的部分被首先考虑到
 * 这是由于set的特征保证的，即 PointsNumCompare 类中定义的，面积小且只有1个特征点的区域，会被放在set的更后面的位置
 * 只获取区域内第一个关键点，是由m_keyPoints中的数据类型set决定的，即 ResponseCompare 类定义了set中响应值高的在前面
 * @param   keypoints   输出关键点
 * @return  true        关键点个数满足要求
 * @return  false       关键点个数不满足要求
 */
bool QuadTree::getKeyPoints(std::vector<cv::KeyPoint> &keypoints) {
    if (!m_matchCond) {
        return false;
    }

    int n = 0;
    for (const auto &node : m_nodes) {
        if (n++ == m_reqNum) {
            break;
        }
        keypoints.push_back(**(node->m_keyPoints.begin()));
    }
    return true;
}

NAMESAPCE_END