#pragma once
#include <SDL2/SDL.h>

struct PositionConfig
{
    float x;
    float y;
};

struct MovementConfig
{
    float speed;
};

struct CollisionConfig
{
    float width;
    float height;
};

struct BehaviorConfig
{
    int behaviorType;
};

struct EntityConfig
{
    PositionConfig* positionConfig;
    MovementConfig* movementConfig;
    CollisionConfig* collisionConfig;
    BehaviorConfig* behaviorConfig;
    
    EntityConfig()
    {
        positionConfig = nullptr;
        movementConfig = nullptr;
        collisionConfig = nullptr;
        behaviorConfig = nullptr;
    }
    
    ~EntityConfig()
    {
        delete positionConfig;
        delete movementConfig;
        delete collisionConfig;
        delete behaviorConfig;
    }
};

struct EntityHandle
{
    int entityID;
    
    EntityHandle()
    {
        entityID = -1;
    }
    
    EntityHandle(int id)
    {
        entityID = id;
    }
    
    bool IsValid() const
    {
        return entityID != -1;
    }
};