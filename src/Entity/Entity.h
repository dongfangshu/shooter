class Entity
{
private:
    static int instanceCount;
    int instanceID;
public:
    Entity(/* args */);
    ~Entity();
    int GetInstanceID() { return instanceID; }
};

Entity::Entity(/* args */)
{
    instanceID = instanceCount++;
}

Entity::~Entity()
{
}
