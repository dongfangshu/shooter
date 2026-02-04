#pragma once
#include "Component.h"
#include "EntityConfig.h"

class CollisionComponent : public Component
{
private:
    float width;
    float height;

public:
    CollisionComponent(float width, float height)
    {
        this->width = width;
        this->height = height;
    }
    
    CollisionComponent(CollisionConfig* config)
    {
        this->width = config->width;
        this->height = config->height;
    }
    
    float GetWidth() const { return width; }
    float GetHeight() const { return height; }
};