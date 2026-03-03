#include "PlayerBehavior.h"
#include <memory>
#include "../Entity/Entity.h"
#include "../Entity/PositionComponent.h"
#include "../Entity/MoveComponent.h"
#include "../Input/InputManager.h"
#include "../Core/ScreenConstants.h"
#include "../Entity/EntityManager.h"
#include "BulletBehavior.h"
void PlayerBehavior::Start()
{
}

// 检查玩家是否可以向指定方向移动
bool PlayerBehavior::CanMoveInDirection(PositionComponent *position, SDL_FPoint direction, float width, float height)
{
    float x = position->GetX();
    float y = position->GetY();

    // 检查上方向
    if (direction.y < 0)
    {
        return y + direction.y > 0;
    }
    // 检查下方向
    if (direction.y > 0)
    {
        return y + direction.y + height < SCREEN_HEIGHT;
    }
    // 检查左方向
    if (direction.x < 0)
    {
        return x + direction.x > 0;
    }
    // 检查右方向
    if (direction.x > 0)
    {
        return x + direction.x + width < SCREEN_WIDTH;
    }

    return true; // 无移动方向
}

void PlayerBehavior::Update()
{
    if (!entity)
        return;
    CheckMove();
    CheckFire();

}
void PlayerBehavior::CheckMove()
{
    InputManager *inputManager = InputManager::GetInstance();
    auto position = entity->GetComponent<PositionComponent>();
    auto move = entity->GetComponent<MoveComponent>();
    auto collision = entity->GetComponent<CollisionComponent>();
    SDL_FPoint moveDelta = {0, 0};
    
    // 检测键盘输入，只传递方向（不包含速度值）
    // 速度值现在由 MoveComponent 管理并乘以 deltaTime
    if (inputManager->IsKeyPressed(SDLK_w))
    {
        moveDelta = SDL_FPoint{0, -1.0f};  // 向上移动
    }
    if (inputManager->IsKeyPressed(SDLK_s))
    {
        moveDelta = SDL_FPoint{0, 1.0f};   // 向下移动
    }
    if (inputManager->IsKeyPressed(SDLK_a))
    {
        moveDelta = SDL_FPoint{-1.0f, 0};  // 向左移动
    }
    if (inputManager->IsKeyPressed(SDLK_d))
    {
        moveDelta = SDL_FPoint{1.0f, 0};   // 向右移动
    }
    
    // 处理斜向移动（同时按下两个方向键）
    if (inputManager->IsKeyPressed(SDLK_w) && inputManager->IsKeyPressed(SDLK_a))
        moveDelta = SDL_FPoint{-0.707f, -0.707f};
    else if (inputManager->IsKeyPressed(SDLK_w) && inputManager->IsKeyPressed(SDLK_d))
        moveDelta = SDL_FPoint{0.707f, -0.707f};
    else if (inputManager->IsKeyPressed(SDLK_s) && inputManager->IsKeyPressed(SDLK_a))
        moveDelta = SDL_FPoint{-0.707f, 0.707f};
    else if (inputManager->IsKeyPressed(SDLK_s) && inputManager->IsKeyPressed(SDLK_d))
        moveDelta = SDL_FPoint{0.707f, 0.707f};
    
    // 归一化后的方向向量乘以速度（在 MoveComponent 中处理）
    if ((moveDelta.x != 0 || moveDelta.y != 0) && 
        CanMoveInDirection(position, moveDelta, collision->GetWidth(), collision->GetHeight()))
    {
        move->DoMove(moveDelta);
    }
}
void PlayerBehavior::CheckFire()
{
    InputManager *inputManager = InputManager::GetInstance();
    if (inputManager->IsKeyPressed(SDLK_SPACE))
    {
        // 发射子弹
        auto position = entity->GetComponent<PositionComponent>();
        auto collision = entity->GetComponent<CollisionComponent>();
        auto direction = position->GetForward();
        
        // 从玩家中心发射
        float spawnX = position->GetX() + collision->GetWidth() / 2;
        float spawnY = position->GetY() + collision->GetHeight() / 2;
        
        auto bulletConfig = std::make_unique<EntityConfig>();
        bulletConfig->positionConfig = std::make_unique<PositionConfig>();
        bulletConfig->positionConfig->x = spawnX;
        bulletConfig->positionConfig->y = spawnY;
        bulletConfig->movementConfig = std::make_unique<MovementConfig>();
        bulletConfig->movementConfig->speed = 600.0f;  // 子弹每秒移动 600 像素
        bulletConfig->collisionConfig = std::make_unique<CollisionConfig>(10.0f, 10.0f, false, false);
        bulletConfig->renderConfig = std::make_unique<RenderConfig>();
        bulletConfig->renderConfig->renderOrder = 100;
        bulletConfig->renderConfig->texturePath = "assets/image/bullet.png";
        bulletConfig->behaviorConfig = std::make_unique<BehaviorConfig>();
        bulletConfig->behaviorConfig->behaviors.push_back(std::make_unique<BulletBehavior>());
        EntityManager::GetInstance()->AddEntity(std::move(bulletConfig));
        
    }
}
