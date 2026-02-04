#pragma once
#include "Component.h"
#include "EntityConfig.h"

class MovementComponent : public Component
{
private:
    float speed;

public:
    MovementComponent(float speed)
    {
        this->speed = speed;
    }
    
    MovementComponent(MovementConfig* config)
    {
        this->speed = config->speed;
    }
    
    float GetSpeed() const { return speed; }
    void SetSpeed(float newSpeed) { speed = newSpeed; }
};