#include "RenderComponent.h"
#include "Entity.h"
#include "RenderManager.h"
#include "../Debug/Debug.h"

RenderComponent::RenderComponent(Entity* entity) : Component(entity), renderOrder(0), texture(nullptr), color({255, 255, 255, 255}), useTexture(false)
{
    // 注册到RenderManager
    RenderManager* renderManager = RenderManager::GetInstance();
    renderManager->RegisterComponent(this);
}

RenderComponent::~RenderComponent()
{
    // 从RenderManager注销
    RenderManager* renderManager = RenderManager::GetInstance();
    renderManager->UnregisterComponent(this);
}

void RenderComponent::SetTexture(SDL_Texture* texture) {
    this->texture = texture;
    this->useTexture = (texture != nullptr);
}

SDL_Texture* RenderComponent::GetTexture() const {
    return texture;
}

void RenderComponent::SetColor(SDL_Color color) {
    this->color = color;
}

SDL_Color RenderComponent::GetColor() const {
    return color;
}

void RenderComponent::SetRenderOrder(int order) {
    this->renderOrder = order;
}

int RenderComponent::GetRenderOrder() const {
    return renderOrder;
}
