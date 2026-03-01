#pragma once
#include "../Scene/Scene.h"
#include <SDL2/SDL.h>
#include "../Entity/EntityConfig.h"
#include "../Entity/CollisionManager.h"

class Canvas;
class GText;

class GameScene : public Scene
{
private:
    SDL_Renderer* renderer;
    EntityHandle playerHandle;
    CollisionManager* collisionManager;
    Canvas* canvas;
    GText* debugText;
    
public:
    GameScene(SDL_Renderer* renderer);
    ~GameScene();
    
    DEFINE_SCENE_NAME(GameScene);
    void Init() override;
    void Update() override;
    void Render(SDL_Renderer* renderer) override;
    void Clear() override;
    Canvas* GetCanvas() override;
};