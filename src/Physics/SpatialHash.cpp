#include "SpatialHash.h"

namespace Physics {

SpatialHash::SpatialHash(float cellSize) : cellSize(cellSize) {
}

std::vector<SpatialHash::CellKey> SpatialHash::GetCellsForAABB(const AABB& bounds) const {
    std::vector<CellKey> result;
    
    int minCellX = WorldToCell(bounds.GetMinX());
    int maxCellX = WorldToCell(bounds.GetMaxX());
    int minCellY = WorldToCell(bounds.GetMinY());
    int maxCellY = WorldToCell(bounds.GetMaxY());
    
    result.reserve((maxCellX - minCellX + 1) * (maxCellY - minCellY + 1));
    
    for (int x = minCellX; x <= maxCellX; ++x) {
        for (int y = minCellY; y <= maxCellY; ++y) {
            result.emplace_back(x, y);
        }
    }
    
    return result;
}

void SpatialHash::Insert(int entityID, const AABB& bounds) {
    // 先移除旧的
    Remove(entityID);
    
    auto cellKeys = GetCellsForAABB(bounds);
    entityCells[entityID] = cellKeys;
    
    for (const auto& key : cellKeys) {
        cells[key].insert(entityID);
    }
}

void SpatialHash::Update(int entityID, const AABB& bounds) {
    // 更新就是重新插入
    Insert(entityID, bounds);
}

void SpatialHash::Remove(int entityID) {
    auto it = entityCells.find(entityID);
    if (it == entityCells.end()) return;
    
    for (const auto& cellKey : it->second) {
        auto cellIt = cells.find(cellKey);
        if (cellIt != cells.end()) {
            cellIt->second.erase(entityID);
            // 如果Cell空了，可以选择删除它
            if (cellIt->second.empty()) {
                cells.erase(cellIt);
            }
        }
    }
    
    entityCells.erase(it);
}

std::vector<int> SpatialHash::Query(const AABB& region) const {
    std::unordered_set<int> uniqueResults;
    auto cellKeys = GetCellsForAABB(region);
    
    for (const auto& key : cellKeys) {
        auto it = cells.find(key);
        if (it != cells.end()) {
            uniqueResults.insert(it->second.begin(), it->second.end());
        }
    }
    
    return std::vector<int>(uniqueResults.begin(), uniqueResults.end());
}

void SpatialHash::Query(const AABB& region, std::function<void(int)> callback) const {
    std::unordered_set<int> visited;
    auto cellKeys = GetCellsForAABB(region);
    
    for (const auto& key : cellKeys) {
        auto it = cells.find(key);
        if (it != cells.end()) {
            for (int entityID : it->second) {
                if (visited.insert(entityID).second) {
                    callback(entityID);
                }
            }
        }
    }
}

// SDL_FRect 重载
std::vector<int> SpatialHash::Query(const SDL_FRect& region) const {
    return Query(AABB(region.x, region.y, region.w, region.h));
}

void SpatialHash::Query(const SDL_FRect& region, std::function<void(int)> callback) const {
    Query(AABB(region.x, region.y, region.w, region.h), callback);
}

std::vector<int> SpatialHash::QueryCell(int cellX, int cellY) const {
    CellKey key(cellX, cellY);
    auto it = cells.find(key);
    if (it != cells.end()) {
        return std::vector<int>(it->second.begin(), it->second.end());
    }
    return {};
}

void SpatialHash::Clear() {
    cells.clear();
    entityCells.clear();
}

} // namespace Physics
