#pragma once

class Entity;

class BaseBehavior
{
public:
    Entity* entity;
    virtual void Start() {}
    virtual void Update() {}
    virtual void Destroy() {}
    virtual void OnCollisionEnter(Entity* other) {}
    virtual void OnCollisionExit(Entity* other) {}
};


