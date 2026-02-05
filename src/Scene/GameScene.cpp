#include "GameScene.h"
#include "../Entity/EntityManager.h"
#include "../Entity/EntityConfig.h"
#include "../Entity/PositionComponent.h"
#include "../Entity/MovementComponent.h"
#include "../Entity/CollisionComponent.h"
#include "../Entity/BehaviorComponent.h"
#include "../Input/InputManager.h"
#include "../Asset/AssetManager.h"
#include "../Debug/Debug.h"

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
    
    EntityConfig* playerConfig = new EntityConfig();
    
    playerConfig->positionConfig = new PositionConfig();
    playerConfig->positionConfig->x = 400.0f;
    playerConfig->positionConfig->y = 500.0f;
    
    playerConfig->movementConfig = new MovementConfig();
    playerConfig->movementConfig->speed = 5.0f;
    
    playerConfig->collisionConfig = new CollisionConfig();
    playerConfig->collisionConfig->width = 64.0f;
    playerConfig->collisionConfig->height = 64.0f;
    
    playerConfig->behaviorConfig = new BehaviorConfig();
    playerConfig->behaviorConfig->behaviorType = 1;
    
    playerHandle = entityManager->AddEntity(playerConfig);
    
    Entity* player = entityManager->GetEntity(playerHandle.entityID);
    if (player != nullptr)
    {
        PositionComponent* position = player->GetComponent<PositionComponent>();
        MovementComponent* movement = player->GetComponent<MovementComponent>();
        
        InputManager* inputManager = InputManager::GetInstance();
        
        inputManager->RegisterKeyPress(SDLK_w, [position, movement]() {
            if (position != nullptr && movement != nullptr)
            {
                position->Move(0, -movement->GetSpeed());
            }
        });
        
        inputManager->RegisterKeyPress(SDLK_s, [position, movement]() {
            if (position != nullptr && movement != nullptr)
            {
                position->Move(0, movement->GetSpeed());
            }
        });
        
        inputManager->RegisterKeyPress(SDLK_a, [position, movement]() {
            if (position != nullptr && movement != nullptr)
            {
                position->Move(-movement->GetSpeed(), 0);
            }
        });
        
        inputManager->RegisterKeyPress(SDLK_d, [position, movement]() {
            if (position != nullptr && movement != nullptr)
            {
                position->Move(movement->GetSpeed(), 0);
            }
        });
    }
    
    delete playerConfig;
}

void GameScene::Update()
{
    EntityManager* entityManager = EntityManager::GetInstance();
    entityManager->Update();
    collisionManager->Update();
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    Entity* player = entityManager->GetEntity(playerHandle.entityID);
    if (player != nullptr)
    {
        PositionComponent* position = player->GetComponent<PositionComponent>();
        CollisionComponent* collision = player->GetComponent<CollisionComponent>();
        
        if (position != nullptr && collision != nullptr)
        {
            SDL_Rect playerRect = {
                (int)position->GetX(),
                (int)position->GetY(),
                (int)collision->GetWidth(),
                (int)collision->GetHeight()
            };
            
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &playerRect);
        }
    }
    
    SDL_RenderPresent(renderer);
}

void GameScene::Clear()
{
    InputManager* inputManager = InputManager::GetInstance();
    inputManager->UnregisterKeyPress(SDLK_w);
    inputManager->UnregisterKeyPress(SDLK_s);
    inputManager->UnregisterKeyPress(SDLK_a);
    inputManager->UnregisterKeyPress(SDLK_d);
    
    EntityManager::DestroyInstance();
}