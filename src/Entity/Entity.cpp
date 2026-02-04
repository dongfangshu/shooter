#include "Entity.h"

int Entity::instanceCount = 0;

Entity::Entity()
{
    instanceID = instanceCount++;
}

Entity::~Entity()
{
    for (auto& pair : components)
    {
        delete pair.second;
    }
    components.clear();
}