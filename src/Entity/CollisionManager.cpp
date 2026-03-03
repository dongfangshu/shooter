#include "CollisionManager.h"
#include "CollisionComponent.h"
#include "Entity.h"
#include "EntityManager.h"
#include "PositionComponent.h"
#include "../Physics/SpatialHash.h"
#include "../Physics/Shape.h"
#include "../Physics/PhysicsQuery.h"
#include "../Physics/PhysicsQuery.h"
#include <algorithm>
#include <unordered_set>

CollisionManager* CollisionManager::instance = nullptr;

CollisionManager* CollisionManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new CollisionManager();
    }
    return instance;
}

void CollisionManager::DestroyInstance()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

CollisionManager::CollisionManager()
    : spatialHash(new Physics::SpatialHash(128.0f))  // Cell 大小 128
{
}

CollisionManager::~CollisionManager()
{
    delete spatialHash;
}

void CollisionManager::AddCollisionComponent(CollisionComponent* component)
{
    collisionComponents.push_back(component);
}

void CollisionManager::RemoveCollisionComponent(CollisionComponent* component)
{
    auto it = std::find(collisionComponents.begin(), collisionComponents.end(), component);
    if (it != collisionComponents.end())
    {
        collisionComponents.erase(it);
    }
}

void CollisionManager::RegisterCollisionCallback(int entityID, CollisionCallback callback)
{
    UnregisterCollisionCallback(entityID);
    collisionCallbacks.emplace_back(entityID, callback);
}

void CollisionManager::UnregisterCollisionCallback(int entityID)
{
    collisionCallbacks.erase(
        std::remove_if(collisionCallbacks.begin(), collisionCallbacks.end(),
            [entityID](const auto& pair) { return pair.first == entityID; }),
        collisionCallbacks.end()
    );
}

void CollisionManager::UpdateSpatialHash()
{
    spatialHash->Clear();
    
    for (auto* component : collisionComponents)
    {
        if (!component) continue;
        
        Entity* entity = component->GetEntity();
        if (!entity) continue;
        
        PositionComponent* pos = entity->GetComponent<PositionComponent>();
        if (!pos) continue;
        
        // 构建 AABB 并插入空间哈希
        Physics::AABB bounds(
            pos->GetX(),
            pos->GetY(),
            component->GetWidth(),
            component->GetHeight()
        );
        
        spatialHash->Insert(entity->GetInstanceID(), bounds);
    }
}

std::vector<std::pair<CollisionComponent*, CollisionComponent*>> CollisionManager::BroadPhase() const
{
    std::vector<std::pair<CollisionComponent*, CollisionComponent*>> potentialCollisions;
    std::unordered_set<uint64_t> checkedPairs;
    
    for (auto* componentA : collisionComponents)
    {
        if (!componentA) continue;
        
        Entity* entityA = componentA->GetEntity();
        if (!entityA) continue;
        
        PositionComponent* posA = entityA->GetComponent<PositionComponent>();
        if (!posA) continue;
        
        // 查询空间哈希获取候选
        Physics::AABB queryBounds(
            posA->GetX(),
            posA->GetY(),
            componentA->GetWidth(),
            componentA->GetHeight()
        );
        
        auto candidates = spatialHash->Query(queryBounds);
        
        for (int entityIDB : candidates)
        {
            if (entityIDB == entityA->GetInstanceID()) continue;
            
            // 生成唯一键避免重复检测
            int idA = entityA->GetInstanceID();
            int idB = entityIDB;
            if (idA > idB) std::swap(idA, idB);
            uint64_t pairKey = (static_cast<uint64_t>(idA) << 32) | static_cast<uint64_t>(idB);
            
            if (checkedPairs.insert(pairKey).second)
            {
                Entity* entityB = EntityManager::GetInstance()->GetEntity(entityIDB);
                if (!entityB) continue;
                
                CollisionComponent* componentB = entityB->GetComponent<CollisionComponent>();
                if (!componentB) continue;
                
                potentialCollisions.emplace_back(componentA, componentB);
            }
        }
    }
    
    return potentialCollisions;
}

bool CollisionManager::NarrowPhaseCheck(CollisionComponent* a, CollisionComponent* b) const
{
    Entity* entityA = a->GetEntity();
    Entity* entityB = b->GetEntity();
    
    if (!entityA || !entityB) return false;
    
    PositionComponent* posA = entityA->GetComponent<PositionComponent>();
    PositionComponent* posB = entityB->GetComponent<PositionComponent>();
    
    if (!posA || !posB) return false;
    
    // AABB 相交检测
    Physics::AABB boundsA(posA->GetX(), posA->GetY(), a->GetWidth(), a->GetHeight());
    Physics::AABB boundsB(posB->GetX(), posB->GetY(), b->GetWidth(), b->GetHeight());
    
    return boundsA.IntersectsAABB(boundsB);
}

void CollisionManager::TriggerCollisionEvent(int entityID1, int entityID2)
{
    for (const auto& [entityID, callback] : collisionCallbacks)
    {
        if (entityID == entityID1)
        {
            Entity* self = EntityManager::GetInstance()->GetEntity(entityID1);
            Entity* other = EntityManager::GetInstance()->GetEntity(entityID2);
            if (self && other && callback)
            {
                callback(self, other);
            }
        }
        else if (entityID == entityID2)
        {
            Entity* self = EntityManager::GetInstance()->GetEntity(entityID2);
            Entity* other = EntityManager::GetInstance()->GetEntity(entityID1);
            if (self && other && callback)
            {
                callback(self, other);
            }
        }
    }
}

void CollisionManager::Update()
{
    // 1. 更新空间哈希
    UpdateSpatialHash();
    
    // 2. 宽相检测获取候选碰撞对
    auto potentialCollisions = BroadPhase();
    
    // 3. 清除当前帧碰撞记录
    currentFrameCollisions.clear();
    
    // 4. 窄相检测并处理碰撞
    for (auto& [compA, compB] : potentialCollisions)
    {
        if (NarrowPhaseCheck(compA, compB))
        {
            Entity* entityA = compA->GetEntity();
            Entity* entityB = compB->GetEntity();
            
            if (!entityA || !entityB) continue;
            
            int idA = entityA->GetInstanceID();
            int idB = entityB->GetInstanceID();
            
            // 记录碰撞对
            if (idA > idB) std::swap(idA, idB);
            currentFrameCollisions.emplace_back(idA, idB);
            
            // 触发碰撞事件
            TriggerCollisionEvent(entityA->GetInstanceID(), entityB->GetInstanceID());
        }
    }
    
    // 5. 更新碰撞记录
    lastFrameCollisions = currentFrameCollisions;
}

std::vector<CollisionComponent*> CollisionManager::QueryRegion(const Physics::AABB& region) const
{
    std::vector<CollisionComponent*> results;
    auto entityIDs = spatialHash->Query(region);
    
    for (int id : entityIDs)
    {
        Entity* entity = EntityManager::GetInstance()->GetEntity(id);
        if (entity)
        {
            CollisionComponent* comp = entity->GetComponent<CollisionComponent>();
            if (comp) results.push_back(comp);
        }
    }
    
    return results;
}

std::vector<CollisionComponent*> CollisionManager::QueryCircle(const SDL_FPoint& center, float radius) const
{
    Physics::Circle circle(center.x, center.y, radius);
    std::vector<CollisionComponent*> results;
    
    auto entityIDs = spatialHash->Query(circle.GetBoundingBox());
    
    for (int id : entityIDs)
    {
        Entity* entity = EntityManager::GetInstance()->GetEntity(id);
        if (!entity) continue;
        
        PositionComponent* pos = entity->GetComponent<PositionComponent>();
        CollisionComponent* comp = entity->GetComponent<CollisionComponent>();
        if (!pos || !comp) continue;
        
        // 精确检测
        Physics::AABB entityBounds(pos->GetX(), pos->GetY(), comp->GetWidth(), comp->GetHeight());
        if (entityBounds.IntersectsCircle(circle))
        {
            results.push_back(comp);
        }
    }
    
    return results;
}

bool CollisionManager::CheckCollision(int entityID1, int entityID2) const
{
    Entity* entity1 = EntityManager::GetInstance()->GetEntity(entityID1);
    Entity* entity2 = EntityManager::GetInstance()->GetEntity(entityID2);
    return CheckCollision(entity1, entity2);
}

bool CollisionManager::CheckCollision(Entity* entity1, Entity* entity2) const
{
    if (!entity1 || !entity2) return false;
    
    CollisionComponent* comp1 = entity1->GetComponent<CollisionComponent>();
    CollisionComponent* comp2 = entity2->GetComponent<CollisionComponent>();
    
    if (!comp1 || !comp2) return false;
    
    return NarrowPhaseCheck(comp1, comp2);
}

bool CollisionManager::Raycast(const Physics::Ray& ray, int& outEntityID, float& outDistance) const
{
    auto results = Physics::PhysicsQuery::Raycast(ray);
    if (!results.empty())
    {
        outEntityID = results[0].entityID;
        outDistance = results[0].distance;
        return true;
    }
    return false;
}

void CollisionManager::Clear()
{
    collisionComponents.clear();
    collisionCallbacks.clear();
    lastFrameCollisions.clear();
    currentFrameCollisions.clear();
    if (spatialHash)
    {
        spatialHash->Clear();
    }
}