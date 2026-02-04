#include "Entity.h"
#include <vector>
class EntityManager
{
private:
    std::vector<Entity*> entities;
public:
    EntityManager(/* args */);
    ~EntityManager();
    void AddEntity(Entity* entity);
    Entity* GetEntity(int instanceID);
    void RemoveEntity(int instanceID);

};
