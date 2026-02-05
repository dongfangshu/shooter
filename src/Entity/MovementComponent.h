#pragma once
#include "Component.h"
#include "EntityConfig.h"

class MovementComponent : public Component
{
private:
    float speed;

public:
    MovementComponent(Entity* entity, float speed) : Component(entity)
    {
        this->speed = speed;
    }
    
    MovementComponent(Entity* entity, MovementConfig* config) : Component(entity)
    {
        this->speed = config->speed;
    }
    
    float GetSpeed() const { return speed; }
    void SetSpeed(float newSpeed) { speed = newSpeed; }
};