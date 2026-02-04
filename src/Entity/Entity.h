#pragma once
#include "Component.h"
#include "PositionComponent.h"
#include "MovementComponent.h"
#include "CollisionComponent.h"
#include "BehaviorComponent.h"
#include <unordered_map>
#include <typeinfo>

class Entity
{
private:
    static int instanceCount;
    int instanceID;
    std::unordered_map<size_t, Component*> components;

public:
    Entity();
    ~Entity();
    
    int GetInstanceID() const { return instanceID; }
    
    template<typename T>
    T* GetComponent()
    {
        size_t typeHash = typeid(T).hash_code();
        auto it = components.find(typeHash);
        if (it != components.end())
        {
            return static_cast<T*>(it->second);
        }
        return nullptr;
    }
    
    template<typename T>
    void AddComponent(T* component)
    {
        size_t typeHash = typeid(T).hash_code();
        components[typeHash] = component;
    }
    
    void Update()
    {
        for (auto& pair : components)
        {
            pair.second->Update();
        }
    }
};