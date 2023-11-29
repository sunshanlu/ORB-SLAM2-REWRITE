#pragma once

#include "common_include.h"

NAMESAPCE_BEGIN
class QuadTree;
class PointsNumCompare;

/// 定义关键点比较器，要求在set中，响应值高的在前
class ResponseCompare {
public:
    bool operator()(const cv::KeyPoint *lhs, const cv::KeyPoint *rhs) const { return lhs->response > rhs->response; }
};

class QuadTreeNode {
    friend class QuadTree;
    friend class PointsNumCompare;

public:
    QuadTreeNode(float minX, float maxX, float minY, float maxY, QuadTreeNode *parent);

    QuadTreeNode(float minX, float maxX, float minY, float maxY, const std::vector<cv::KeyPoint> &keypoints);

    /// 删除拷贝构造函数
    QuadTreeNode(const QuadTreeNode &other) = delete;
    /// 删除拷贝赋值运算符
    QuadTreeNode &operator=(const QuadTreeNode &other) = delete;

private:
    QuadTreeNode *m_parent = nullptr;
    QuadTreeNode *m_children[4] = {nullptr};                          ///< 四个子节点指针
    float m_minX, m_maxX, m_minY, m_maxY;                             ///< 表示节点的矩形区域
    std::multiset<const cv::KeyPoint *, ResponseCompare> m_keyPoints; ///< 该节点包含的所有特征点
};

/// 定义关键点数量比较器，要求在set中，数量多的在前
class PointsNumCompare {
public:
    bool operator()(const QuadTreeNode *lhs, const QuadTreeNode *rhs) const {
        return lhs->m_keyPoints.size() > rhs->m_keyPoints.size();
    }
};

class QuadTree {
public:
    QuadTree(const cv::Mat &image, const std::vector<cv::KeyPoint> &keypoints, int num);

    ~QuadTree() {
        for (auto node : m_allNodes) {
            delete node;
        }
        m_allNodes.clear();
    }

    bool getKeyPoints(std::vector<cv::KeyPoint> &keypoints);

private:
    void split(QuadTreeNode *parent);

    QuadTreeNode *m_root;                                    ///< 根节点
    std::multiset<QuadTreeNode *, PointsNumCompare> m_nodes; ///< 目前没有子节点的节点
    int m_num = 0;                                           ///< m_nodes中的节点数
    bool m_matchCond = true;                                 ///< 是否满足要求num条件
    int m_reqNum;                                            ///< 要求的节点数
    std::vector<QuadTreeNode *> m_allNodes; ///< 存储所有构造在堆区的节点，以便进行析构
};

NAMESAPCE_END