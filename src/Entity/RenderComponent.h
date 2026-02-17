#pragma once
#include "Component.h"
#include <SDL2/SDL.h>
#include "EntityConfig.h"

class RenderComponent:public Component
{
private:
    int renderOrder;
    SDL_Texture* texture;
    SDL_Color color;
    bool useTexture;

public:
    RenderComponent(Entity* entity,RenderConfig* config);
    ~RenderComponent();

    void SetTexture(SDL_Texture* texture);
    SDL_Texture* GetTexture() const;
    void SetColor(SDL_Color color);
    SDL_Color GetColor() const;
    void SetRenderOrder(int order);
    int GetRenderOrder() const;
};
