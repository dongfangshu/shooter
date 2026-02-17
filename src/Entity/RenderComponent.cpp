#include "RenderComponent.h"
#include "Entity.h"
#include "RenderManager.h"
#include "../Debug/Debug.h"
#include "../Asset/AssetManager.h"

RenderComponent::RenderComponent(Entity* entity,RenderConfig* config) : Component(entity)
{
    this->texture = AssetManager::GetInstance()->LoadTextureAtPath(config->texturePath);
    this->renderOrder = config->renderOrder;
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
