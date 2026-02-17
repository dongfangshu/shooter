#include "BehaviorComponent.h"
BehaviorComponent::BehaviorComponent(Entity* entity, BehaviorConfig* config) : Component(entity)
{
    for (auto behavior : config->behaviors)
    {
        behavior->entity = entity;
        behavior->Start();
        behaviors.push_back(behavior);
    }
}
void BehaviorComponent::Update()
{
    for (auto behavior : behaviors)
    {
        behavior->Update();
    }
}

BehaviorComponent::~BehaviorComponent()
{
    for (auto behavior : behaviors)
    {
        behavior->Destroy();
        delete behavior;
    }
}

