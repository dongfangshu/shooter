#pragma once
#include "Shape.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>

namespace Physics {

// 空间哈希（Cell划分）用于粗筛
class SpatialHash {
public:
    // cellSize: 每个格子的大小
    SpatialHash(float cellSize = 64.0f);
    ~SpatialHash() = default;

    // 插入/更新实体
    void Insert(int entityID, const AABB& bounds);
    void Update(int entityID, const AABB& bounds);
    void Remove(int entityID);

    // 查询与给定区域重叠的实体ID（去重）
    std::vector<int> Query(const AABB& region) const;
    std::vector<int> Query(const SDL_FRect& region) const;
    
    // 查询与给定区域重叠的实体ID（使用回调，避免分配）
    void Query(const AABB& region, std::function<void(int)> callback) const;
    void Query(const SDL_FRect& region, std::function<void(int)> callback) const;

    // 获取指定位置的Cell中的所有实体
    std::vector<int> QueryCell(int cellX, int cellY) const;

    // 清除所有数据
    void Clear();

    // 调试：获取Cell数量
    size_t GetCellCount() const { return cells.size(); }

private:
    using CellKey = std::pair<int, int>;
    
    struct CellKeyHash {
        size_t operator()(const CellKey& key) const {
            // 简单的哈希组合
            return std::hash<int>()(key.first) ^ (std::hash<int>()(key.second) << 1);
        }
    };

    float cellSize;
    std::unordered_map<CellKey, std::unordered_set<int>, CellKeyHash> cells;
    std::unordered_map<int, std::vector<CellKey>> entityCells;  // 记录每个实体在哪些Cell中

    // 计算AABB覆盖的所有Cell坐标
    std::vector<CellKey> GetCellsForAABB(const AABB& bounds) const;
    
    // 世界坐标转Cell坐标
    inline int WorldToCell(float worldCoord) const {
        return static_cast<int>(std::floor(worldCoord / cellSize));
    }
};

} // namespace Physics
