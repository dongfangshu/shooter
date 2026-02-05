#include "CollisionManager.h"
#include <algorithm>

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
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Update()
{
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