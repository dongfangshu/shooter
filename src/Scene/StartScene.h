#pragma once
#include "Scene.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Stage;
class AssetManager;

class StartScene : public Scene {
private:
    SDL_Renderer* renderer;
    Stage* stage{nullptr};

    static const int SCREEN_W = 800;
    static const int SCREEN_H = 600;

public:
    StartScene(SDL_Renderer* renderer);
    ~StartScene();

    void Init() override;
    void Update() override;
    void Clear() override;
    Stage* GetStage() override;
};
