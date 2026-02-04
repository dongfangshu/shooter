#pragma once
#include "../Scene/Scene.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class StartScene : public Scene
{
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    
public:
    StartScene(SDL_Renderer* renderer);
    ~StartScene();
    
    void Init() override;
    void Update() override;
    void Clear() override;
};