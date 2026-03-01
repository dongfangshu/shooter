#include "game.h"
#include "Asset/AssetManager.h"
#include "Debug/Debug.h"
#include "Event/EventManager.h"
#include "Scene/SceneManager.h"
#include "Input/InputManager.h"
#include "GUI/UI/Canvas.h"
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
    // 退出前恢复原始键盘布局（如果 InputManager 已初始化）
    if (InputManager::GetInstance() != nullptr)
    {
        InputManager::GetInstance()->RestoreOriginalInputMethod();
        InputManager::DestroyInstance();
    }
    
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
        RenderLoop();
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
    
    // 禁用文本输入模式，确保游戏按键（如 WASD）不被中文输入法拦截
    SDL_StopTextInput();
    Debug::Log("SDL_StopTextInput - disabled text input for game controls");
    
    // 初始化输入管理器
    Debug::Log("Initializing InputManager...");
    InputManager::GetInstance()->Initialize();
    Debug::Log("InputManager initialized");
    
    // 注册所有场景
    Debug::Log("Registering scenes...");
    auto startScene = new StartScene(render);
    SceneManager::AddScene(startScene->GetName(), startScene);
    Debug::Log("StartScene registered");
    auto gameScene = new GameScene(render);
    SceneManager::AddScene(gameScene->GetName(), gameScene);
    Debug::Log("GameScene registered");
    auto endScene = new EndScene(render);
    SceneManager::AddScene(endScene->GetName(), endScene);
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
        SceneManager::currentScene->Update();
        Canvas* canvas = SceneManager::currentScene->GetCanvas();
        if (canvas)
            canvas->InternalUpdate(frameEvents);
    }
}

void Game::RenderLoop()
{
    if (SceneManager::currentScene != nullptr)
    {
        SceneManager::currentScene->Render(render);
    }
}