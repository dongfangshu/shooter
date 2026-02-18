#pragma once
#include "../Entity/BaseBehavior.h"
#include "../Entity/PositionComponent.h"
#include "../Entity/MoveComponent.h"
#include "../Entity/CollisionComponent.h"
#include "SDL2/SDL.h"

class BulletBehavior:public BaseBehavior
{
private:
    /* data */
public:
    BulletBehavior(/* args */);
    ~BulletBehavior();
    void Update() override;
    void OnCollisionEnter(EntityHandle other) override;
};

