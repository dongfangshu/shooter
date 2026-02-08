#include "game.h"
#include "Asset/AssetManager.h"
#include "Debug/Debug.h"
#include "Event/EventManager.h"
#include "Scene/SceneManager.h"
#include "Input/InputManager.h"
#include "GUI/UI/Stage.h"
#include "GUI/UI/UIConfig.h"
int FRAME = 0;
const int FRAME_RATE = 60; // 帧率
// bool isRunning = false;
Game::Game(SDL_Window *window)
{
    this->window = window;
    //this->assetManager = nullptr;
}
Game::~Game()
{
    InputManager::DestroyInstance();
    
    //if (assetManager != nullptr) {
    //    delete assetManager;
    //    assetManager = nullptr;
    //}
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
        std::vector<SDL_Event> frameEvents;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            frameEvents.push_back(event);
            EventManager::onSDLEvent.emit({&event});
            InputManager::GetInstance()->HandleEvent(event);
        }
        LogicUpdate(frameEvents);
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
    AssetManager::GetInstance()->Init(render);
    
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
    

    
    EventManager::onSDLEvent += [this](const SDLEventData& data)
    {
        if (data.sdlEvent->type == SDL_QUIT)
        {
            Debug::Error("Quit Game");
            this->isRunning = false;
        }
    };
    // 初始化GUI
    Debug::Log("Initializing GUI...");
    defaultFont = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 48);
    if (defaultFont == nullptr) {
        Debug::Error("Failed to load font: " + std::string(TTF_GetError()));
        return false;
    }
    Debug::Log("GUI initialized");
    // 设置初始场景
    Debug::Log("Changing to StartScene...");
    SceneManager::ChangeScene("StartScene");
    Debug::Log("StartScene initialized");
    return true;
}
void Game::LogicUpdate(const std::vector<SDL_Event>& frameEvents)
{
    if (SceneManager::currentScene != nullptr)
    {
        Stage* stage = SceneManager::currentScene->GetStage();
        if (stage)
            stage->InternalUpdate(frameEvents);
        SceneManager::currentScene->Update();
    }
}

void Game::RenderLoop()
{
    SDL_RenderClear(render);
    // 渲染游戏元素
    SDL_RenderPresent(render);
}