#pragma once

class Entity;

class BaseBehavior
{
public:
    virtual void Start() {}
    virtual void Update() {}
    virtual void Destroy() {}
    virtual void OnCollisionEnter(Entity* other) {}
    virtual void OnCollisionExit(Entity* other) {}
};


