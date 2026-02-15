#include "GameScene.h"
#include "../Entity/EntityManager.h"
#include "../Entity/EntityConfig.h"
#include "../Entity/PositionComponent.h"
#include "../Entity/MovementComponent.h"
#include "../Entity/CollisionComponent.h"
#include "../Entity/BehaviorComponent.h"
#include "../Entity/RenderComponent.h"
#include "../Entity/RenderManager.h"
#include "../Input/InputManager.h"
#include "../Asset/AssetManager.h"
#include "../Debug/Debug.h"
#include "../GamePlay/Player.h"

GameScene::GameScene(SDL_Renderer* renderer)
{
    this->renderer = renderer;
    this->collisionManager = nullptr;
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
    
    // 获取玩家实体并添加RenderComponent
    Entity* playerEntity = player->GetPlayerEntity();
    if (playerEntity) {
        RenderComponent* renderComponent = new RenderComponent(playerEntity);
        playerEntity->AddComponent(renderComponent);
    }
}

void GameScene::Update()
{
    EntityManager* entityManager = EntityManager::GetInstance();
    entityManager->Update();
    collisionManager->Update();
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    // 调用RenderManager的Update进行渲染
    RenderManager* renderManager = RenderManager::GetInstance();
    renderManager->Update();
    
    SDL_RenderPresent(renderer);
}

void GameScene::Clear()
{
    Player::DestroyInstance();
    RenderManager::DestroyInstance();
    EntityManager::DestroyInstance();
}