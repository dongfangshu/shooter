#pragma once
#include "../Entity/BaseBehavior.h"

class Entity;

class PlayerBehavior : public BaseBehavior {
private:
    Entity* entity;
    float speed;

public:
    PlayerBehavior(Entity* entity);
    ~PlayerBehavior();

    void Start() override;
    void Update() override;
    void SetSpeed(float speed);
    float GetSpeed() const;
};