#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <memory>

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
    bool isStatic = false;   // 是否为静态物体
    bool isTrigger = false;  // 是否为触发器
};

struct BehaviorConfig
{
    std::vector<std::unique_ptr<BaseBehavior>> behaviors;
};
struct RenderConfig
{
    std::string texturePath;
    int renderOrder;
};
struct EntityConfig
{
    std::unique_ptr<PositionConfig> positionConfig;
    std::unique_ptr<MovementConfig> movementConfig;
    std::unique_ptr<CollisionConfig> collisionConfig;
    std::unique_ptr<BehaviorConfig> behaviorConfig;
    std::unique_ptr<RenderConfig> renderConfig;
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
