#include "RenderManager.h"
#include "RenderComponent.h"
#include "Entity.h"
#include "PositionComponent.h"
#include "CollisionComponent.h"
#include "../Debug/Debug.h"
#include <algorithm>

RenderManager* RenderManager::instance = nullptr;

RenderManager::RenderManager() : renderer(nullptr) {
}

RenderManager::~RenderManager() {
    renderComponents.clear();
}

RenderManager* RenderManager::GetInstance() {
    if (!instance) {
        instance = new RenderManager();
    }
    return instance;
}

void RenderManager::DestroyInstance() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

void RenderManager::Init(SDL_Renderer* renderer) {
    this->renderer = renderer;
}

void RenderManager::Update() {
    if (!renderer) return;

    // 排序组件
    SortComponents();

    // 渲染所有组件
    for (auto component : renderComponents) {
        if (!component) continue;

        Entity* entity = component->GetEntity();
        if (!entity) continue;

        PositionComponent* position = entity->GetComponent<PositionComponent>();
        CollisionComponent* collision = entity->GetComponent<CollisionComponent>();

        if (position && collision) {
            SDL_Rect rect = {
                (int)position->GetX(),
                (int)position->GetY(),
                (int)collision->GetWidth(),
                (int)collision->GetHeight()
            };

            SDL_Texture* texture = component->GetTexture();
            if (texture) {
                SDL_RenderCopy(renderer, texture, nullptr, &rect);
            } else {
                SDL_Color color = component->GetColor();
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void RenderManager::RegisterComponent(RenderComponent* component) {
    if (component) {
        renderComponents.push_back(component);
    }
}

void RenderManager::UnregisterComponent(RenderComponent* component) {
    if (!component) return;

    auto it = std::find(renderComponents.begin(), renderComponents.end(), component);
    if (it != renderComponents.end()) {
        renderComponents.erase(it);
    }
}

void RenderManager::SortComponents() {
    // 按renderOrder排序，值越小渲染越靠前
    std::sort(renderComponents.begin(), renderComponents.end(), [](RenderComponent* a, RenderComponent* b) {
        return a->GetRenderOrder() < b->GetRenderOrder();
    });
}
