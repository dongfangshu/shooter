#pragma once
#include "../Scene/Scene.h"
#include <SDL2/SDL.h>
#include "../Entity/EntityConfig.h"
#include "../Entity/CollisionManager.h"

class GameScene : public Scene
{
private:
    SDL_Renderer* renderer;
    EntityHandle playerHandle;
    CollisionManager* collisionManager;
    
public:
    GameScene(SDL_Renderer* renderer);
    ~GameScene();
    
    void Init() override;
    void Update() override;
    void Clear() override;
};