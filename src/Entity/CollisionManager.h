#pragma once

#include <vector>
#include <functional>
#include "../Physics/Shape.h"
#include "../Physics/SpatialHash.h"

class CollisionComponent;
class Entity;

// 碰撞回调函数类型
using CollisionCallback = std::function<void(Entity* self, Entity* other)>;

class CollisionManager
{
private:
    std::vector<CollisionComponent*> collisionComponents;
    static CollisionManager* instance;
    
    // 碰撞事件回调
    std::vector<std::pair<int, CollisionCallback>> collisionCallbacks;
    
    // 使用空间哈希进行宽相检测
    class Physics::SpatialHash* spatialHash;
    
    // 上一帧的碰撞对（用于检测进入/退出）
    std::vector<std::pair<int, int>> lastFrameCollisions;
    std::vector<std::pair<int, int>> currentFrameCollisions;

public:
    static CollisionManager* GetInstance();
    static void DestroyInstance();
    
    CollisionManager();
    ~CollisionManager();
    
    // 主更新循环
    void Update();
    
    // 注册/注销碰撞组件
    void AddCollisionComponent(CollisionComponent* component);
    void RemoveCollisionComponent(CollisionComponent* component);
    
    // 注册碰撞回调
    void RegisterCollisionCallback(int entityID, CollisionCallback callback);
    void UnregisterCollisionCallback(int entityID);
    
    // 空间查询 API
    std::vector<CollisionComponent*> QueryRegion(const Physics::AABB& region) const;
    std::vector<CollisionComponent*> QueryCircle(const SDL_FPoint& center, float radius) const;
    
    // 检测两个实体是否碰撞
    bool CheckCollision(int entityID1, int entityID2) const;
    bool CheckCollision(Entity* entity1, Entity* entity2) const;
    
    // 射线检测
    bool Raycast(const Physics::Ray& ray, int& outEntityID, float& outDistance) const;
    
    // 获取所有碰撞组件
    const std::vector<CollisionComponent*>& GetAllComponents() const { return collisionComponents; }
    
    // 清空所有数据
    void Clear();

private:
    // 窄相检测：精确检测两个组件是否碰撞
    bool NarrowPhaseCheck(CollisionComponent* a, CollisionComponent* b) const;
    
    // 触发碰撞事件
    void TriggerCollisionEvent(int entityID1, int entityID2);
    
    // 更新空间哈希
    void UpdateSpatialHash();
    
    // 宽相检测：获取可能碰撞的候选对
    std::vector<std::pair<CollisionComponent*, CollisionComponent*>> BroadPhase() const;
};


