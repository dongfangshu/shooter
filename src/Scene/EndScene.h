#pragma once
#include "../Scene/Scene.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class EndScene : public Scene
{
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    
public:
    EndScene(SDL_Renderer* renderer);
    ~EndScene();
    
    void Init() override;
    void Update() override;
    void Clear() override;
};