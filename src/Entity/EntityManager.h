#pragma once
#include "Entity.h"
#include "EntityConfig.h"
#include "PositionComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "BehaviorComponent.h"
#include "RenderComponent.h"
#include <unordered_map>

class EntityManager
{
private:
    static EntityManager* instance;
    std::unordered_map<int, Entity*> entities;
    int nextEntityID;
    
    EntityManager();
    ~EntityManager();

public:
    static EntityManager* GetInstance();
    static void DestroyInstance();
    
    EntityHandle AddEntity(EntityConfig* config);
    Entity* GetEntity(int entityID);
    void RemoveEntity(int entityID);
    void Update();
    void LateUpdate();
    
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;
};