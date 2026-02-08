#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <functional>

class UpdateContext {
private:
    SDL_Renderer* renderer;
    std::vector<std::function<void(SDL_Renderer*)>> renderCallbacks;
    
public:
    UpdateContext(SDL_Renderer* renderer);
    ~UpdateContext();
    
    void Begin();
    void End();
    
    void AddRenderCallback(std::function<void(SDL_Renderer*)> callback);
    void ClearCallbacks();
    
    SDL_Renderer* GetRenderer() const;
};