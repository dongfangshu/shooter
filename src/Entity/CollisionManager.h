#pragma once

#include <vector>

class CollisionComponent;

class CollisionManager
{
private:
    std::vector<CollisionComponent*> collisionComponents;
    static CollisionManager* instance;
public:
    static CollisionManager* GetInstance();
    static void DestroyInstance();
    CollisionManager();
    ~CollisionManager();
    void Update();
    void AddCollisionComponent(CollisionComponent* component);
    void RemoveCollisionComponent(CollisionComponent* component);
};


