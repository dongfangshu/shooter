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

public:
    StartScene(SDL_Renderer* renderer);
    ~StartScene();

    DEFINE_SCENE_NAME(StartScene);
    void Init() override;
    void Update() override;
    void Clear() override;
    Canvas* GetCanvas() override;
};
