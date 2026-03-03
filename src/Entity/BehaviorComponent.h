#pragma once
#include "Component.h"
#include "EntityConfig.h"
#include "BaseBehavior.h"
#include <memory>
#include <vector>

class BehaviorComponent : public Component
{
private:
    std::vector<std::unique_ptr<BaseBehavior>> behaviors;

public:
    BehaviorComponent(Entity* entity, BehaviorConfig* config);
    ~BehaviorComponent() = default;
    void Update();
};