#pragma once
#include <SDL2/SDL.h>
#include <functional>
#include <unordered_map>

class InputManager
{
private:
    static InputManager* instance;
    std::unordered_map<SDL_Keycode, std::function<void()>> keyPressCallbacks;
    std::unordered_map<SDL_Keycode, bool> keyStates;
    
    InputManager();
    ~InputManager();

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