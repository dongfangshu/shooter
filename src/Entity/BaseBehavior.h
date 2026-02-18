#pragma once

#include "Entity.h"
#include "EntityConfig.h"

class BaseBehavior
{
public:
    Entity* entity;
    virtual void Start() {}
    virtual void Update() {}
    virtual void Destroy() {}
    virtual void OnCollisionEnter(EntityHandle other) {}
    virtual void OnCollisionExit(EntityHandle other) {}
};


