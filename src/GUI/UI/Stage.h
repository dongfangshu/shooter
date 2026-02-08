#pragma once
#include "UIComponent.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

class UpdateContext;

struct StageConfig {

};

class Stage : public UIComponent {
private:
    UIComponent* lastMotionTarget{nullptr};
    StageConfig config;
    void HandleEvent(const SDL_Event& event);

public:
    Stage(SDL_Renderer* renderer, int width, int height);
    ~Stage();

    void InternalUpdate(const std::vector<SDL_Event>& events);

    void Update(UpdateContext* ctx) override;
};
