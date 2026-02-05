#pragma once
#include "Component.h"
#include "EntityConfig.h"
#include "CollisionManager.h"

class CollisionComponent : public Component
{
private:
    PositionComponent* positionComponent;
    float width;
    float height;
    
public:
    
    CollisionComponent(CollisionConfig* config, Entity* entity) : Component(entity)
    {
        this->width = config->width;
        this->height = config->height;
        CollisionManager::GetInstance()->AddCollisionComponent(this);
    }
    ~CollisionComponent()
    {
        CollisionManager::GetInstance()->RemoveCollisionComponent(this);
    }

    float GetWidth() const { return width; }
    float GetHeight() const { return height; }
};