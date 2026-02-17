#pragma once
#include "Component.h"
#include "EntityConfig.h"
#include "SDL2/SDL.h"

class MoveComponent : public Component
{
private:
    float speed;

public:
    MoveComponent(Entity* entity, float speed) : Component(entity)
    {
        this->speed = speed;
    }
    
    MoveComponent(Entity* entity, MovementConfig* config) : Component(entity)
    {
        this->speed = config->speed;
    }
    
    float GetSpeed() const { return speed; }
    void SetSpeed(float newSpeed) { speed = newSpeed; }
    void DoMove(SDL_FPoint offset);
    void LateUpdate() override;
};