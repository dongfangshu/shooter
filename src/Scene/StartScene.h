#pragma once
#include "Scene.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Canvas;
class AssetManager;

class StartScene : public Scene {
private:
    SDL_Renderer* renderer;
    Canvas* canvas{nullptr};

    static const int SCREEN_W = 800;
    static const int SCREEN_H = 600;

public:
    StartScene(SDL_Renderer* renderer);
    ~StartScene();

    void Init() override;
    void Update() override;
    void Clear() override;
    Canvas* GetCanvas() override;
};
