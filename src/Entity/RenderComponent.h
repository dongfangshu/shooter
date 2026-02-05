#include "Component.h"
class RenderComponent:public Component
{
private:
    int renderOrder;
public:
    RenderComponent(Entity* entity);
    ~RenderComponent();
};

RenderComponent::RenderComponent(Entity* entity) : Component(entity), renderOrder(0)
{
}

RenderComponent::~RenderComponent()
{
}
