#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "Asset/AssetManager.h"
#include "Scene/SceneManager.h"
#include "Scene/StartScene.h"
#include "Scene/GameScene.h"
#include "Scene/EndScene.h"
#include "Input/InputManager.h"

class Game
{
    SDL_Window *window;
    SDL_Renderer *render;
    //AssetManager *assetManager;

public:
    Game(SDL_Window *window);
    ~Game();
    void Run();
    bool Init();

private:
    bool isRunning = false;
    void LogicUpdate(const std::vector<SDL_Event>& frameEvents);
    void RenderLoop();
};