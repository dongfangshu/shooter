#include "GameScene.h"
#include "../Entity/EntityManager.h"
#include "../Entity/EntityConfig.h"
#include "../Entity/PositionComponent.h"
#include "../Entity/MoveComponent.h"
#include "../Entity/CollisionComponent.h"
#include "../Entity/BehaviorComponent.h"
#include "../Entity/RenderComponent.h"
#include "../Entity/RenderManager.h"
#include "../Input/InputManager.h"
#include "../Asset/AssetManager.h"
#include "../Debug/Debug.h"
#include "../GamePlay/Player.h"
#include "../GUI/UI/Canvas.h"
#include "../GUI/UI/GText.h"
#include "../Core/ScreenConstants.h"

GameScene::GameScene(SDL_Renderer* renderer)
{
    this->renderer = renderer;
    this->collisionManager = nullptr;
    this->canvas = nullptr;
    this->debugText = nullptr;
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
    EntityManager* entityManager = EntityManager::GetInstance();
    collisionManager = CollisionManager::GetInstance();
    
    // 初始化RenderManager
    RenderManager* renderManager = RenderManager::GetInstance();
    renderManager->Init(renderer);
    
    // 初始化Player
    Player* player = Player::GetInstance();
    player->Init(renderer);
    
    // 初始化Canvas用于显示debug信息
    canvas = new Canvas(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // 创建debug文本显示entity数量
    debugText = new GText("Entities: 0");
    debugText->SetPosition({10, 10});
    SDL_Color textColor = {0, 255, 0, 255};
    debugText->SetColor(textColor);
    canvas->AddChild(debugText);
}

void GameScene::Update()
{
    EntityManager* entityManager = EntityManager::GetInstance();
    entityManager->Update();
    collisionManager->Update();

    entityManager->LateUpdate();
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    // 调用RenderManager的Update进行渲染
    RenderManager* renderManager = RenderManager::GetInstance();
    renderManager->Update();
    
    // 更新debug文本显示当前entity数量
    if (debugText)
    {
        int entityCount = entityManager->GetEntityCount();
        debugText->SetText("Entities: " + std::to_string(entityCount));
    }
    
    SDL_RenderPresent(renderer);
}

void GameScene::Clear()
{
    Player::DestroyInstance();
    RenderManager::DestroyInstance();
    EntityManager::DestroyInstance();
}

Canvas* GameScene::GetCanvas()
{
    return canvas;
}