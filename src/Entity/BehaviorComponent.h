#pragma once
#include "Component.h"
#include "EntityConfig.h"
#include "BaseBehavior.h"
class BehaviorComponent : public Component
{
private:
    std::vector<BaseBehavior*> behaviors;

public:
    BehaviorComponent(Entity* entity, BehaviorConfig* config);
    ~BehaviorComponent();
    void Update();
};