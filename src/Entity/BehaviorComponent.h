#pragma once
#include "Component.h"
#include "EntityConfig.h"

class BehaviorComponent : public Component
{
private:
    int behaviorType;

public:
    BehaviorComponent(int type)
    {
        this->behaviorType = type;
    }
    
    BehaviorComponent(BehaviorConfig* config)
    {
        this->behaviorType = config->behaviorType;
    }
    
    int GetBehaviorType() const { return behaviorType; }
    void SetBehaviorType(int type) { behaviorType = type; }
};