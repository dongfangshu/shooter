#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>

// 前向声明
class BaseBehavior;
struct PositionConfig
{
    float x;
    float y;
    float angle;
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
    // int behaviorType;
    std::vector<BaseBehavior*> behaviors;
};
struct RenderConfig
{
    std::string texturePath;
    int renderOrder;
};
struct EntityConfig
{
    PositionConfig* positionConfig;
    MovementConfig* movementConfig;
    CollisionConfig* collisionConfig;
    BehaviorConfig* behaviorConfig;
    RenderConfig* renderConfig;
    
    EntityConfig()
    {
        positionConfig = nullptr;
        movementConfig = nullptr;
        collisionConfig = nullptr;
        behaviorConfig = nullptr;
        renderConfig = nullptr;
    }
    
    ~EntityConfig()
    {
        delete positionConfig;
        delete movementConfig;
        delete collisionConfig;
        delete behaviorConfig;
        delete renderConfig;
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
