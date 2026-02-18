#pragma once
#include "Component.h"
#include "EntityConfig.h"

class PositionComponent : public Component
{
private:
    float x;
    float y;
    float angle;

public:
    PositionComponent(PositionConfig *config, Entity *entity) : Component(entity)
    {
        this->x = config->x;
        this->y = config->y;
        this->angle = config->angle;
    }

    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetAngle() const { return angle; }

    void SetX(float newX) { x = newX; }
    void SetY(float newY) { y = newY; }
    void SetAngle(float newAngle) { angle = newAngle; }

    void Move(float dx, float dy)
    {
        x += dx;
        y += dy;
    }
    SDL_FPoint GetPosition() const { return {x, y}; }
    void SetPosition(SDL_FPoint position){ x = position.x;y = position.y;}
    SDL_FPoint GetForward()
    {
        float rad = angle * M_PI / 180.0f;
        return {(float)cos(rad), (float)sin(rad)};
    }
    SDL_FPoint GetBack()
    {
        float rad = angle * M_PI / 180.0f;
        return {-(float)cos(rad), -(float)sin(rad)};
    }
    SDL_FPoint GetLeft()
    {
        float rad = angle * M_PI / 180.0f;
        return {(float)sin(rad), -(float)cos(rad)};
    }
    SDL_FPoint GetRight()
    {
        float rad = angle * M_PI / 180.0f;
        return {-(float)sin(rad), (float)cos(rad)};
    }
};