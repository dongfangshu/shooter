#pragma once
#include "Component.h"
#include "EntityConfig.h"
#include "BaseBehavior.h"
class BehaviorComponent : public Component
{
private:
    BehaviorConfig* config;
    BaseBehavior* behavior;

public:
    BehaviorComponent(Entity* entity, BehaviorConfig* config);
    ~BehaviorComponent();
    void Update();
    void SetBehavior(BaseBehavior* behavior);
    BaseBehavior* GetBehavior() const;
};