#pragma once
#include <SDL2/SDL.h>
#include <functional>
#include <unordered_map>
#include <queue>

#ifdef _WIN32
#include <windows.h>
#endif

class InputManager
{
private:
    static InputManager* instance;
    std::unordered_map<SDL_Keycode, std::function<void()>> keyPressCallbacks;
    std::unordered_map<SDL_Keycode, bool> keyStates;
    std::queue<SDL_Event> eventQueue;
    
#ifdef _WIN32
    HKL originalKeyboardLayout;  // 保存原始键盘布局
#endif
    
    InputManager();
    ~InputManager();

public:
    // 初始化时调用，切换到英文输入法
    void Initialize();

    // 退出时恢复原始键盘布局
    void RestoreOriginalInputMethod();

private:
    // 切换到英文输入法（仅Windows）
    void SwitchToEnglishInputMethod();

public:
    static InputManager* GetInstance();
    static void DestroyInstance();
    
    void HandleEvent(const SDL_Event& event);
    void RegisterKeyPress(SDL_Keycode key, std::function<void()> callback);
    void UnregisterKeyPress(SDL_Keycode key);
    
    bool IsKeyPressed(SDL_Keycode key) const;
    void Update();
    
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;
};