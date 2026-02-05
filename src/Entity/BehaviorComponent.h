#pragma once
#include "Component.h"
#include "EntityConfig.h"
#include "BaseBehavior.h"
class BehaviorComponent : public Component
{
private:
    BehaviorConfig* config;

public:
    BehaviorComponent(Entity* entity, BehaviorConfig* config);
    ~BehaviorComponent();
    void Update();
};