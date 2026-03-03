#pragma once
#include "Shape.h"
#include <vector>
#include <memory>
#include <functional>

namespace Physics {

// 四叉树节点数据
struct QuadTreeNodeData {
    int entityID;
    AABB bounds;
    
    QuadTreeNodeData(int id, const AABB& b) : entityID(id), bounds(b) {}
};

// 四叉树节点
class QuadTreeNode {
public:
    static const int MAX_OBJECTS = 8;    // 每个节点最多容纳的对象数
    static const int MAX_DEPTH = 5;      // 最大深度

    QuadTreeNode(const AABB& bounds, int depth = 0);
    ~QuadTreeNode() = default;

    // 插入对象
    bool Insert(const QuadTreeNodeData& data);
    
    // 从树中移除对象
    bool Remove(int entityID);
    
    // 更新对象位置
    bool Update(int entityID, const AABB& newBounds);

    // 查询与区域重叠的对象
    void Query(const AABB& region, std::vector<int>& results) const;
    void Query(const AABB& region, std::function<void(int)> callback) const;

    // 射线检测
    void Raycast(const Ray& ray, std::vector<int>& results) const;

    // 清除所有对象和子节点
    void Clear();

    // 获取节点边界
    const AABB& GetBounds() const { return bounds; }
    
    // 是否是叶节点
    bool IsLeaf() const { return !nw && !ne && !sw && !se; }

private:
    AABB bounds;
    int depth;
    std::vector<QuadTreeNodeData> objects;
    
    // 四个子节点
    std::unique_ptr<QuadTreeNode> nw;  // 西北
    std::unique_ptr<QuadTreeNode> ne;  // 东北
    std::unique_ptr<QuadTreeNode> sw;  // 西南
    std::unique_ptr<QuadTreeNode> se;  // 东南

    void Subdivide();
    int GetQuadrant(const AABB& targetBounds) const;
    bool Contains(const AABB& targetBounds) const;
};

// 四叉树管理器
class QuadTree {
public:
    QuadTree(const AABB& worldBounds);
    ~QuadTree() = default;

    // 插入/更新/移除
    void Insert(int entityID, const AABB& bounds);
    void Update(int entityID, const AABB& newBounds);
    void Remove(int entityID);

    // 查询
    std::vector<int> Query(const AABB& region) const;
    void Query(const AABB& region, std::function<void(int)> callback) const;
    
    // 射线检测
    std::vector<int> Raycast(const Ray& ray) const;

    // 清除
    void Clear();

    // 重建树（当大量对象移动后）
    void Rebuild(const std::vector<std::pair<int, AABB>>& allObjects);

private:
    std::unique_ptr<QuadTreeNode> root;
};

} // namespace Physics
