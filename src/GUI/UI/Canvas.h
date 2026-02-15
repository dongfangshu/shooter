#pragma once
#include "UIComponent.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

class UpdateContext;

struct CanvasConfig {

};

class Canvas : public UIComponent {
private:
    UIComponent* lastMotionTarget{nullptr};
    CanvasConfig config;
    void HandleEvent(const SDL_Event& event);

public:
    Canvas(SDL_Renderer* renderer, int width, int height);
    ~Canvas();

    void InternalUpdate(const std::vector<SDL_Event>& events);

    void Update(UpdateContext* ctx) override;
};
