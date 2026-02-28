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
    // 同步创建实体
    auto entity = new Entity();
    int entityID = nextEntityID++;
    
    if (config->positionConfig != nullptr)
    {
        entity->AddComponent(new PositionComponent(config->positionConfig, entity));
    }
    
    if (config->movementConfig != nullptr)
    {
        entity->AddComponent(new MoveComponent(entity, config->movementConfig));
    }
    
    if (config->collisionConfig != nullptr)
    {
        entity->AddComponent(new CollisionComponent(config->collisionConfig, entity));
    }
    
    if (config->behaviorConfig != nullptr)
    {
        entity->AddComponent(new BehaviorComponent(entity, config->behaviorConfig));
    }
    if (config->renderConfig != nullptr)
    {
        entity->AddComponent(new RenderComponent(entity,config->renderConfig));
    }
    
    entities[entityID] = entity;
    
    // 删除配置
    delete config;
    
    return EntityHandle(entityID);
}

void EntityManager::ProcessPendingEntities()
{
    // 处理待移除的实体（延迟移除）
    while (!pendingRemoveEntities.empty())
    {
        int entityID = pendingRemoveEntities.front();
        pendingRemoveEntities.pop();
        
        auto it = entities.find(entityID);
        if (it != entities.end())
        {
            delete it->second;
            entities.erase(it);
        }
    }
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
    // 将实体ID加入待移除队列
    pendingRemoveEntities.push(entityID);
}

void EntityManager::Update()
{
    // 处理队列中的延迟移除
    ProcessPendingEntities();
    
    // 更新所有实体
    for (auto& pair : entities)
    {
        pair.second->Update();
    }
}
void EntityManager::LateUpdate()
{
    for (auto& pair : entities)
    {
        pair.second->LateUpdate();
    }
}

size_t EntityManager::GetEntityCount() const
{
    return entities.size();
}