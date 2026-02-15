#pragma once
class Entity;

class Component
{
private:
    Entity *entity;

public:
    // 定义组件迭代顺序

    Component(Entity *entity)
    {
        this->entity = entity;
    }
    virtual ~Component() {}
    virtual void Update() {}
    
    Entity* GetEntity() const {
        return entity;
    }
};