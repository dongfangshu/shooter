#include "game.h"
#include "Asset/AssetManager.h"
#include "Debug/Debug.h"
#include "Event/EventManager.h"
#include "Scene/SceneManager.h"
#include "Input/InputManager.h"
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
    InputManager::DestroyInstance();
    
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
            InputManager::GetInstance()->HandleEvent(event);
        }
        
        // 更新场景
        LogicUpdate();
        
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
    
    // 初始化输入管理器
    Debug::Log("Initializing InputManager...");
    InputManager::GetInstance();
    Debug::Log("InputManager initialized");
    
    // 注册所有场景
    Debug::Log("Registering scenes...");
    SceneManager::AddScene("StartScene", new StartScene(render));
    Debug::Log("StartScene registered");
    SceneManager::AddScene("GameScene", new GameScene(render));
    Debug::Log("GameScene registered");
    SceneManager::AddScene("EndScene", new EndScene(render));
    Debug::Log("EndScene registered");
    
    // 设置初始场景
    Debug::Log("Changing to StartScene...");
    SceneManager::ChangeScene("StartScene");
    Debug::Log("StartScene initialized");
    
    EventManager::onSDLEvent += [this](const SDLEventData& data)
    {
        if (data.sdlEvent->type == SDL_QUIT)
        {
            Debug::Error("Quit Game");
            this->isRunning = false;
        }
    };
    return true;
}
void Game::LogicUpdate()
{
    if (SceneManager::currentScene != nullptr)
    {
        SceneManager::currentScene->Update();
    }
}

void Game::RenderLoop()
{
    SDL_RenderClear(render);
    // 渲染游戏元素
    SDL_RenderPresent(render);
}