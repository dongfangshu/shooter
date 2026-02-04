#include "EntityManager.h"

EntityManager* EntityManager::instance = nullptr;

EntityManager::EntityManager()
{
    nextEntityID = 0;
}

EntityManager::~EntityManager()
{
    for (auto& pair : entities)
    {
        delete pair.second;
    }
    entities.clear();
}

EntityManager* EntityManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new EntityManager();
    }
    return instance;
}

void EntityManager::DestroyInstance()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

EntityHandle EntityManager::AddEntity(EntityConfig* config)
{
    auto entity = new Entity();
    int entityID = nextEntityID++;
    
    if (config->positionConfig != nullptr)
    {
        entity->AddComponent(new PositionComponent(config->positionConfig));
    }
    
    if (config->movementConfig != nullptr)
    {
        entity->AddComponent(new MovementComponent(config->movementConfig));
    }
    
    if (config->collisionConfig != nullptr)
    {
        entity->AddComponent(new CollisionComponent(config->collisionConfig));
    }
    
    if (config->behaviorConfig != nullptr)
    {
        entity->AddComponent(new BehaviorComponent(config->behaviorConfig));
    }
    
    entities[entityID] = entity;
    return EntityHandle(entityID);
}

Entity* EntityManager::GetEntity(int entityID)
{
    auto it = entities.find(entityID);
    if (it != entities.end())
    {
        return it->second;
    }
    return nullptr;
}

void EntityManager::RemoveEntity(int entityID)
{
    auto it = entities.find(entityID);
    if (it != entities.end())
    {
        delete it->second;
        entities.erase(it);
    }
}

void EntityManager::Update()
{
    for (auto& pair : entities)
    {
        pair.second->Update();
    }
}