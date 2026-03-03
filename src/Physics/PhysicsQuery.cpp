#include "PhysicsQuery.h"
#include "SpatialHash.h"
#include "../Entity/CollisionComponent.h"
#include "../Entity/EntityManager.h"
#include "../Entity/PositionComponent.h"
#include <algorithm>
#include <cmath>

namespace Physics {

// 获取全局 SpatialHash 实例
static SpatialHash& GetGlobalSpatialHash() {
    static SpatialHash instance(128.0f);  // Cell 大小 128
    return instance;
}

void PhysicsQuery::QueryInternal(const AABB& bounds, const Shape* preciseShape, const QueryFilter& filter, std::vector<QueryResult>& results) {
    auto& spatialHash = GetGlobalSpatialHash();
    
    spatialHash.Query(bounds, [&](int entityID) {
        if (!filter.Pass(entityID)) return;
        
        // 获取实体和碰撞组件
        Entity* entity = EntityManager::GetInstance()->GetEntity(entityID);
        if (!entity) return;
        
        CollisionComponent* collision = entity->GetComponent<CollisionComponent>();
        if (!collision) return;
        
        // 精确形状检测
        float distance = 0.0f;
        if (preciseShape) {
            // 获取实体的精确碰撞形状
            // 这里简化处理，使用 AABB 检测
            PositionComponent* pos = entity->GetComponent<PositionComponent>();
            if (!pos) return;
            
            AABB entityAABB(pos->GetX(), pos->GetY(), collision->GetWidth(), collision->GetHeight());
            if (!entityAABB.Intersects(*preciseShape)) return;
            
            distance = CalculateDistance(preciseShape->GetCenter(), entityAABB);
        }
        
        results.push_back({entityID, collision, entity, distance});
    });
}

float PhysicsQuery::CalculateDistance(const SDL_FPoint& point, const Shape& shape) {
    // 简化：计算点到形状中心的距离
    SDL_FPoint center = shape.GetCenter();
    float dx = point.x - center.x;
    float dy = point.y - center.y;
    return std::sqrt(dx * dx + dy * dy);
}

std::vector<QueryResult> PhysicsQuery::QueryShape(const Shape& shape) {
    return QueryShape(shape, QueryFilter{});
}

std::vector<QueryResult> PhysicsQuery::QueryShape(const Shape& shape, const QueryFilter& filter) {
    std::vector<QueryResult> results;
    SDL_FRect rect = shape.GetBoundingBox();
    AABB bounds(rect.x, rect.y, rect.w, rect.h);
    QueryInternal(bounds, &shape, filter, results);
    return results;
}

std::vector<QueryResult> PhysicsQuery::QueryAABB(const AABB& aabb) {
    return QueryAABB(aabb, QueryFilter{});
}

std::vector<QueryResult> PhysicsQuery::QueryAABB(const AABB& aabb, const QueryFilter& filter) {
    std::vector<QueryResult> results;
    QueryInternal(aabb, nullptr, filter, results);
    return results;
}

std::vector<QueryResult> PhysicsQuery::QueryCircle(const SDL_FPoint& center, float radius) {
    return QueryCircle(center, radius, QueryFilter{});
}

std::vector<QueryResult> PhysicsQuery::QueryCircle(const SDL_FPoint& center, float radius, const QueryFilter& filter) {
    Circle circle(center.x, center.y, radius);
    return QueryShape(circle, filter);
}

std::vector<QueryResult> PhysicsQuery::Raycast(const Ray& ray) {
    return Raycast(ray, QueryFilter{});
}

std::vector<QueryResult> PhysicsQuery::Raycast(const Ray& ray, const QueryFilter& filter) {
    std::vector<QueryResult> results;
    auto& spatialHash = GetGlobalSpatialHash();
    
    // 射线包围盒
    SDL_FPoint endPoint{
        ray.origin.x + ray.direction.x * ray.maxDistance,
        ray.origin.y + ray.direction.y * ray.maxDistance
    };
    
    float minX = std::min(ray.origin.x, endPoint.x);
    float maxX = std::max(ray.origin.x, endPoint.x);
    float minY = std::min(ray.origin.y, endPoint.y);
    float maxY = std::max(ray.origin.y, endPoint.y);
    
    AABB rayBounds(minX, minY, maxX - minX, maxY - minY);
    
    spatialHash.Query(rayBounds, [&](int entityID) {
        if (!filter.Pass(entityID)) return;
        
        Entity* entity = EntityManager::GetInstance()->GetEntity(entityID);
        if (!entity) return;
        
        CollisionComponent* collision = entity->GetComponent<CollisionComponent>();
        PositionComponent* pos = entity->GetComponent<PositionComponent>();
        if (!collision || !pos) return;
        
        // 简单的射线-AABB相交检测
        AABB entityAABB(pos->GetX(), pos->GetY(), collision->GetWidth(), collision->GetHeight());
        
        // 射线与AABB相交测试
        float tmin = 0.0f, tmax = ray.maxDistance;
        
        for (int i = 0; i < 2; ++i) {
            float origin = (i == 0) ? ray.origin.x : ray.origin.y;
            float dir = (i == 0) ? ray.direction.x : ray.direction.y;
            float min = (i == 0) ? entityAABB.GetMinX() : entityAABB.GetMinY();
            float max = (i == 0) ? entityAABB.GetMaxX() : entityAABB.GetMaxY();
            
            if (std::abs(dir) < 0.0001f) {
                if (origin < min || origin > max) return;
            } else {
                float ood = 1.0f / dir;
                float t1 = (min - origin) * ood;
                float t2 = (max - origin) * ood;
                if (t1 > t2) std::swap(t1, t2);
                tmin = std::max(tmin, t1);
                tmax = std::min(tmax, t2);
                if (tmin > tmax) return;
            }
        }
        
        if (tmin <= ray.maxDistance) {
            SDL_FPoint hitPoint{
                ray.origin.x + ray.direction.x * tmin,
                ray.origin.y + ray.direction.y * tmin
            };
            
            results.push_back({entityID, collision, entity, tmin});
        }
    });
    
    // 按距离排序
    std::sort(results.begin(), results.end(), [](const QueryResult& a, const QueryResult& b) {
        return a.distance < b.distance;
    });
    
    return results;
}

bool PhysicsQuery::RaycastSingle(const Ray& ray, QueryResult& outResult) {
    return RaycastSingle(ray, QueryFilter{}, outResult);
}

bool PhysicsQuery::RaycastSingle(const Ray& ray, const QueryFilter& filter, QueryResult& outResult) {
    auto results = Raycast(ray, filter);
    if (results.empty()) return false;
    outResult = results[0];
    return true;
}

bool PhysicsQuery::ClosestPoint(const SDL_FPoint& point, QueryResult& outResult) {
    return ClosestPoint(point, QueryFilter{}, outResult);
}

bool PhysicsQuery::ClosestPoint(const SDL_FPoint& point, const QueryFilter& filter, QueryResult& outResult) {
    // 在查询点周围逐步扩大搜索范围
    float searchRadius = 64.0f;
    float maxSearchRadius = 2000.0f;  // 最大搜索范围
    
    while (searchRadius <= maxSearchRadius) {
        auto results = QueryCircle(point, searchRadius, filter);
        if (!results.empty()) {
            // 找到最近的
            QueryResult* closest = nullptr;
            float minDist = FLT_MAX;
            
            for (auto& result : results) {
                PositionComponent* pos = result.entity->GetComponent<PositionComponent>();
                if (!pos) continue;
                
                float dx = pos->GetX() - point.x;
                float dy = pos->GetY() - point.y;
                float dist = std::sqrt(dx * dx + dy * dy);
                
                if (dist < minDist) {
                    minDist = dist;
                    closest = &result;
                }
            }
            
            if (closest) {
                outResult = *closest;
                outResult.distance = minDist;
                return true;
            }
        }
        searchRadius *= 2.0f;
    }
    
    return false;
}

std::vector<QueryResult> PhysicsQuery::LinearCast(const AABB& startBounds, const SDL_FPoint& velocity, float deltaTime, const QueryFilter& filter) {
    // 计算终点位置
    AABB endBounds(
        startBounds.GetX() + velocity.x * deltaTime,
        startBounds.GetY() + velocity.y * deltaTime,
        startBounds.GetWidth(),
        startBounds.GetHeight()
    );
    
    // 合并起点和终点的包围盒
    float minX = std::min(startBounds.GetMinX(), endBounds.GetMinX());
    float maxX = std::max(startBounds.GetMaxX(), endBounds.GetMaxX());
    float minY = std::min(startBounds.GetMinY(), endBounds.GetMinY());
    float maxY = std::max(startBounds.GetMaxY(), endBounds.GetMaxY());
    
    AABB sweptBounds(minX, minY, maxX - minX, maxY - minY);
    
    auto results = QueryAABB(sweptBounds, filter);
    
    // 按与移动方向的距离排序
    for (auto& result : results) {
        PositionComponent* pos = result.entity->GetComponent<PositionComponent>();
        if (pos) {
            float dx = pos->GetX() - startBounds.GetX();
            float dy = pos->GetY() - startBounds.GetY();
            result.distance = dx * velocity.x + dy * velocity.y;  // 投影距离
        }
    }
    
    std::sort(results.begin(), results.end(), [](const QueryResult& a, const QueryResult& b) {
        return a.distance < b.distance;
    });
    
    return results;
}

std::vector<QueryResult> PhysicsQuery::LinearCast(const AABB& startBounds, const SDL_FPoint& velocity, float deltaTime) {
    return LinearCast(startBounds, velocity, deltaTime, QueryFilter{});
}

} // namespace Physics
