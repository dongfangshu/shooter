#pragma once
#include <vector>
#include <SDL2/SDL.h>

// 前向声明
class RenderComponent;

class RenderManager {
private:
    static RenderManager* instance;
    std::vector<RenderComponent*> renderComponents;
    SDL_Renderer* renderer;

    RenderManager();
    ~RenderManager();

public:
    static RenderManager* GetInstance();
    static void DestroyInstance();

    void Init(SDL_Renderer* renderer);
    void Update();
    void RegisterComponent(RenderComponent* component);
    void UnregisterComponent(RenderComponent* component);
    void SortComponents();

    RenderManager(const RenderManager&) = delete;
    RenderManager& operator=(const RenderManager&) = delete;
};