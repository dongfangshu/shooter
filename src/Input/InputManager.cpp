#include "InputManager.h"

InputManager* InputManager::instance = nullptr;

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

InputManager* InputManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new InputManager();
    }
    return instance;
}

void InputManager::DestroyInstance()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

void InputManager::HandleEvent(const SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        SDL_Keycode key = event.key.keysym.sym;
        // eventQueue.push(event);
        keyStates[key] = true;
        
        auto it = keyPressCallbacks.find(key);
        if (it != keyPressCallbacks.end())
        {
            it->second();
        }
    }
    else if (event.type == SDL_KEYUP)
    {
        SDL_Keycode key = event.key.keysym.sym;
        keyStates[key] = false;
    }
}

void InputManager::RegisterKeyPress(SDL_Keycode key, std::function<void()> callback)
{
    keyPressCallbacks[key] = callback;
}

void InputManager::UnregisterKeyPress(SDL_Keycode key)
{
    keyPressCallbacks.erase(key);
}

bool InputManager::IsKeyPressed(SDL_Keycode key) const
{
    auto it = keyStates.find(key);
    if (it != keyStates.end())
    {
        return it->second;
    }
    return false;
}

void InputManager::Update()
{
}