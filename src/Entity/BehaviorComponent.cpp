#include "BehaviorComponent.h"
BehaviorComponent::BehaviorComponent(Entity* entity, BehaviorConfig* config) : Component(entity), config(config), behavior(nullptr)
{
    for (auto behavior : config->behaviors)
    {
        behavior->Start();
    }
}
void BehaviorComponent::Update()
{
    for (auto behavior : config->behaviors)
    {
        behavior->Update();
    }
    if (behavior)
    {
        behavior->Update();
    }
}
void BehaviorComponent::SetBehavior(BaseBehavior* behavior)
{
    this->behavior = behavior;
    if (behavior)
    {
        behavior->Start();
    }
}
BaseBehavior* BehaviorComponent::GetBehavior() const
{
    return behavior;
}
BehaviorComponent::~BehaviorComponent()
{
    for (auto behavior : config->behaviors)
    {
        delete behavior;
    }
    if (behavior)
    {
        delete behavior;
    }
}

