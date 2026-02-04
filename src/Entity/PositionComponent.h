#pragma once
#include "Component.h"
#include "EntityConfig.h"

class PositionComponent : public Component
{
private:
    float x;
    float y;

public:
    PositionComponent(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
    
    PositionComponent(PositionConfig* config)
    {
        this->x = config->x;
        this->y = config->y;
    }
    
    float GetX() const { return x; }
    float GetY() const { return y; }
    
    void SetX(float newX) { x = newX; }
    void SetY(float newY) { y = newY; }
    
    void Move(float dx, float dy)
    {
        x += dx;
        y += dy;
    }
};