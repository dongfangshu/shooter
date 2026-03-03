#include "BehaviorComponent.h"

BehaviorComponent::BehaviorComponent(Entity* entity, BehaviorConfig* config) : Component(entity)
{
    for (auto& behavior : config->behaviors)
    {
        behavior->entity = entity;
        behavior->Start();
        behaviors.push_back(std::move(behavior));
    }
}

void BehaviorComponent::Update()
{
    for (auto& behavior : behaviors)
    {
        behavior->Update();
    }
}