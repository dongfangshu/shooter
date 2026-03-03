#include "QuadTree.h"
#include <algorithm>

namespace Physics {

// ==================== QuadTreeNode ====================

QuadTreeNode::QuadTreeNode(const AABB& bounds, int depth)
    : bounds(bounds), depth(depth) {
    objects.reserve(MAX_OBJECTS);
}

void QuadTreeNode::Subdivide() {
    float x = bounds.GetX();
    float y = bounds.GetY();
    float halfWidth = bounds.GetWidth() * 0.5f;
    float halfHeight = bounds.GetHeight() * 0.5f;

    nw = std::make_unique<QuadTreeNode>(AABB(x, y, halfWidth, halfHeight), depth + 1);
    ne = std::make_unique<QuadTreeNode>(AABB(x + halfWidth, y, halfWidth, halfHeight), depth + 1);
    sw = std::make_unique<QuadTreeNode>(AABB(x, y + halfHeight, halfWidth, halfHeight), depth + 1);
    se = std::make_unique<QuadTreeNode>(AABB(x + halfWidth, y + halfHeight, halfWidth, halfHeight), depth + 1);

    // 将当前对象分配到子节点
    std::vector<QuadTreeNodeData> tempObjects = std::move(objects);
    objects.clear();

    for (const auto& data : tempObjects) {
        Insert(data);
    }
}

int QuadTreeNode::GetQuadrant(const AABB& targetBounds) const {
    float centerX = bounds.GetX() + bounds.GetWidth() * 0.5f;
    float centerY = bounds.GetY() + bounds.GetHeight() * 0.5f;

    bool top = targetBounds.GetMaxY() < centerY;
    bool bottom = targetBounds.GetMinY() > centerY;
    bool left = targetBounds.GetMaxX() < centerX;
    bool right = targetBounds.GetMinX() > centerX;

    if (top && left) return 0;      // NW
    if (top && right) return 1;     // NE
    if (bottom && left) return 2;   // SW
    if (bottom && right) return 3;  // SE
    return -1;  // 跨越多个象限
}

bool QuadTreeNode::Contains(const AABB& targetBounds) const {
    return targetBounds.GetMinX() >= bounds.GetMinX() &&
           targetBounds.GetMaxX() <= bounds.GetMaxX() &&
           targetBounds.GetMinY() >= bounds.GetMinY() &&
           targetBounds.GetMaxY() <= bounds.GetMaxY();
}

bool QuadTreeNode::Insert(const QuadTreeNodeData& data) {
    if (!Contains(data.bounds)) {
        return false;
    }

    // 如果是叶节点且未达到容量上限，直接插入
    if (IsLeaf() && objects.size() < static_cast<size_t>(MAX_OBJECTS)) {
        objects.push_back(data);
        return true;
    }

    // 需要分裂
    if (IsLeaf() && depth < MAX_DEPTH) {
        Subdivide();
    }

    // 尝试插入到子节点
    if (!IsLeaf()) {
        int quadrant = GetQuadrant(data.bounds);
        if (quadrant == 0 && nw->Insert(data)) return true;
        if (quadrant == 1 && ne->Insert(data)) return true;
        if (quadrant == 2 && sw->Insert(data)) return true;
        if (quadrant == 3 && se->Insert(data)) return true;
    }

    // 跨越多个象限或无法继续分裂，留在当前节点
    objects.push_back(data);
    return true;
}

bool QuadTreeNode::Remove(int entityID) {
    // 在当前节点的对象中查找
    auto it = std::find_if(objects.begin(), objects.end(),
        [entityID](const QuadTreeNodeData& data) { return data.entityID == entityID; });
    
    if (it != objects.end()) {
        objects.erase(it);
        return true;
    }

    // 在子节点中查找
    if (!IsLeaf()) {
        if (nw->Remove(entityID)) return true;
        if (ne->Remove(entityID)) return true;
        if (sw->Remove(entityID)) return true;
        if (se->Remove(entityID)) return true;
    }

    return false;
}

bool QuadTreeNode::Update(int entityID, const AABB& newBounds) {
    // 先移除旧对象
    if (!Remove(entityID)) {
        return false;
    }
    
    // 重新插入
    Insert(QuadTreeNodeData(entityID, newBounds));
    return true;
}

void QuadTreeNode::Query(const AABB& region, std::vector<int>& results) const {
    // 检查当前节点的对象
    for (const auto& data : objects) {
        if (data.bounds.IntersectsAABB(region)) {
            results.push_back(data.entityID);
        }
    }

    // 如果区域与节点边界不相交，直接返回
    if (!bounds.IntersectsAABB(region)) {
        return;
    }

    // 递归查询子节点
    if (!IsLeaf()) {
        nw->Query(region, results);
        ne->Query(region, results);
        sw->Query(region, results);
        se->Query(region, results);
    }
}

void QuadTreeNode::Query(const AABB& region, std::function<void(int)> callback) const {
    // 检查当前节点的对象
    for (const auto& data : objects) {
        if (data.bounds.IntersectsAABB(region)) {
            callback(data.entityID);
        }
    }

    // 如果区域与节点边界不相交，直接返回
    if (!bounds.IntersectsAABB(region)) {
        return;
    }

    // 递归查询子节点
    if (!IsLeaf()) {
        nw->Query(region, callback);
        ne->Query(region, callback);
        sw->Query(region, callback);
        se->Query(region, callback);
    }
}

void QuadTreeNode::Raycast(const Ray& ray, std::vector<int>& results) const {
    // TODO: 实现射线与AABB的相交检测
    // 简化为查询包围盒
    AABB rayBounds(ray.origin.x, ray.origin.y, 1, 1);
    Query(rayBounds, results);
}

void QuadTreeNode::Clear() {
    objects.clear();
    nw.reset();
    ne.reset();
    sw.reset();
    se.reset();
}

// ==================== QuadTree ====================

QuadTree::QuadTree(const AABB& worldBounds) {
    root = std::make_unique<QuadTreeNode>(worldBounds, 0);
}

void QuadTree::Insert(int entityID, const AABB& bounds) {
    root->Insert(QuadTreeNodeData(entityID, bounds));
}

void QuadTree::Update(int entityID, const AABB& newBounds) {
    root->Update(entityID, newBounds);
}

void QuadTree::Remove(int entityID) {
    root->Remove(entityID);
}

std::vector<int> QuadTree::Query(const AABB& region) const {
    std::vector<int> results;
    root->Query(region, results);
    return results;
}

void QuadTree::Query(const AABB& region, std::function<void(int)> callback) const {
    root->Query(region, callback);
}

std::vector<int> QuadTree::Raycast(const Ray& ray) const {
    std::vector<int> results;
    root->Raycast(ray, results);
    return results;
}

void QuadTree::Clear() {
    root->Clear();
}

void QuadTree::Rebuild(const std::vector<std::pair<int, AABB>>& allObjects) {
    Clear();
    for (const auto& pair : allObjects) {
        Insert(pair.first, pair.second);
    }
}

} // namespace Physics
