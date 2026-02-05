#include "BehaviorComponent.h"
BehaviorComponent::BehaviorComponent(Entity* entity, BehaviorConfig* config) : Component(entity), config(config)
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
}
BehaviorComponent::~BehaviorComponent()
{
    for (auto behavior : config->behaviors)
    {
        delete behavior;
    }
}

