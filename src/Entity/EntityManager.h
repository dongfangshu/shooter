#pragma once
#include "Entity.h"
#include "EntityConfig.h"
#include "PositionComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "BehaviorComponent.h"
#include "RenderComponent.h"
#include <unordered_map>
#include <queue>

class EntityManager
{
private:
    static EntityManager* instance;
    std::unordered_map<int, Entity*> entities;
    int nextEntityID;
    
    // 延迟处理队列（只用于移除）
    std::queue<int> pendingRemoveEntities;
    
    EntityManager();
    ~EntityManager();

    // 处理队列中的添加和移除
    void ProcessPendingEntities();

public:
    static EntityManager* GetInstance();
    static void DestroyInstance();
    
    EntityHandle AddEntity(EntityConfig* config);
    Entity* GetEntity(int entityID);
    void RemoveEntity(int entityID);
    void Update();
    void LateUpdate();
    size_t GetEntityCount() const;
    
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;
};