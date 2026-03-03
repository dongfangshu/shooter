#pragma once
#include "Shape.h"
#include <functional>
#include <vector>

// 前向声明
class CollisionComponent;
class Entity;

namespace Physics {

// 查询过滤条件
struct QueryFilter {
    bool includeStatic = true;      // 包含静态物体
    bool includeDynamic = true;     // 包含动态物体
    int excludeEntityID = -1;       // 排除指定实体（通常是自己）
    std::function<bool(int)> customFilter;  // 自定义过滤函数

    bool Pass(int entityID) const {
        if (entityID == excludeEntityID) return false;
        if (customFilter && !customFilter(entityID)) return false;
        return true;
    }
};

// 查询结果
struct QueryResult {
    int entityID;
    CollisionComponent* component;
    Entity* entity;
    float distance;  // 到查询形状的距离（用于排序）
};

// 物理查询系统
class PhysicsQuery {
public:
    // 使用形状查询区域内的所有碰撞体
    static std::vector<QueryResult> QueryShape(const Shape& shape);
    static std::vector<QueryResult> QueryShape(const Shape& shape, const QueryFilter& filter);
    
    // 使用AABB查询（性能最好）
    static std::vector<QueryResult> QueryAABB(const AABB& aabb);
    static std::vector<QueryResult> QueryAABB(const AABB& aabb, const QueryFilter& filter);
    
    // 使用圆形查询
    static std::vector<QueryResult> QueryCircle(const SDL_FPoint& center, float radius);
    static std::vector<QueryResult> QueryCircle(const SDL_FPoint& center, float radius, const QueryFilter& filter);
    
    // 射线检测
    static std::vector<QueryResult> Raycast(const Ray& ray);
    static std::vector<QueryResult> Raycast(const Ray& ray, const QueryFilter& filter);
    
    // 获取最近的射线碰撞
    static bool RaycastSingle(const Ray& ray, QueryResult& outResult);
    static bool RaycastSingle(const Ray& ray, const QueryFilter& filter, QueryResult& outResult);
    
    // 点到最近碰撞体的距离
    static bool ClosestPoint(const SDL_FPoint& point, QueryResult& outResult);
    static bool ClosestPoint(const SDL_FPoint& point, const QueryFilter& filter, QueryResult& outResult);
    
    // 线性投射（带移动的碰撞检测）
    static std::vector<QueryResult> LinearCast(const AABB& startBounds, const SDL_FPoint& velocity, float deltaTime);
    static std::vector<QueryResult> LinearCast(const AABB& startBounds, const SDL_FPoint& velocity, float deltaTime, const QueryFilter& filter);

private:
    // 内部查询实现
    static void QueryInternal(const AABB& bounds, const Shape* preciseShape, const QueryFilter& filter, std::vector<QueryResult>& results);
    static float CalculateDistance(const SDL_FPoint& point, const Shape& shape);
};

// 便捷查询函数
namespace Query {
    // 快速矩形查询
    inline std::vector<QueryResult> Box(float x, float y, float width, float height) {
        return PhysicsQuery::QueryAABB(AABB(x, y, width, height));
    }
    
    // 快速圆形查询
    inline std::vector<QueryResult> Circle(float x, float y, float radius) {
        return PhysicsQuery::QueryCircle({x, y}, radius);
    }
    
    // 快速射线查询
    inline std::vector<QueryResult> Ray(float x, float y, float dirX, float dirY, float maxDist = FLT_MAX) {
        return PhysicsQuery::Raycast(Physics::Ray(x, y, dirX, dirY, maxDist));
    }
}

} // namespace Physics
