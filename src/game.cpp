#include "game.h"
#include "Asset/AssetManager.h"
#include "debug.h"
#include "Event/EventManager.h"
int FRAME = 0;
const int FRAME_RATE = 60; // 帧率
// bool isRunning = false;
Game::Game(SDL_Window *window)
{
    this->window = window;
    this->assetManager = nullptr;
}
Game::~Game()
{
    if (assetManager != nullptr) {
        delete assetManager;
        assetManager = nullptr;
    }
    if (render != nullptr) {
        SDL_DestroyRenderer(render);
        render = nullptr;
    }
    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}
void Game::Run()
{
    isRunning = true;
    while (isRunning) {
        // 处理事件
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            EventManager::onSDLEvent.emit({&event});
        }
        // 控制帧率
        SDL_Delay(1000 / FRAME_RATE);
        FRAME++;
    }
}
bool Game::Init()
{
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (render == NULL)
    {
        return false;
    }
    Debug::Log("SDL_CreateRenderer success");
    assetManager = new AssetManager(render);
    EventManager::onSDLEvent += [this](const SDLEventData& data)
    {
        if (data.sdlEvent->type == SDL_QUIT)
        {
            Debug::Error("Quit Game");
        }
        this->isRunning = false;
    };
    return true;
}
void Game::LogicUpdate()
{

}

void Game::RenderLoop()
{
    SDL_RenderClear(render);
    // 渲染游戏元素
    SDL_RenderPresent(render);
}