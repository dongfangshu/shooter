#include "PlayerBehavior.h"
#include "../Entity/Entity.h"
#include "../Entity/PositionComponent.h"
#include "../Entity/Movecomponent.h"
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
    auto speed = move->GetSpeed();
    SDL_FPoint moveDelta = {0, 0};
    // 检测键盘输入并移动玩家
    if (inputManager->IsKeyPressed(SDLK_w))
    {
        moveDelta = SDL_FPoint{0, -speed};
    }
    if (inputManager->IsKeyPressed(SDLK_s))
    {
        moveDelta = SDL_FPoint{0, speed};
    }
    if (inputManager->IsKeyPressed(SDLK_a))
    {
        moveDelta = SDL_FPoint(-speed, 0);
    }
    if (inputManager->IsKeyPressed(SDLK_d))
    {
        moveDelta = SDL_FPoint(speed, 0);
    }
    if ((moveDelta.x != 0 || moveDelta.y != 0) && CanMoveInDirection(position, moveDelta, collision->GetWidth(), collision->GetHeight()))
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
        auto direction = position->GetForward();
        auto spawnPos = position->GetPosition();
        spawnPos.x += direction.x;
        spawnPos.y += direction.y;
        auto bulletConfig = new EntityConfig();
        bulletConfig->positionConfig = new PositionConfig();
        bulletConfig->positionConfig->x = spawnPos.x;
        bulletConfig->positionConfig->y = spawnPos.y;
        bulletConfig->movementConfig = new MovementConfig();
        bulletConfig->movementConfig->speed = 10.0f;
        bulletConfig->collisionConfig = new CollisionConfig();
        bulletConfig->collisionConfig->width = 10.0f;
        bulletConfig->collisionConfig->height = 10.0f;
        bulletConfig->renderConfig = new RenderConfig();
        bulletConfig->renderConfig->renderOrder = 100;
        bulletConfig->renderConfig->texturePath = "assets/image/bullet.png";
        bulletConfig->behaviorConfig = new BehaviorConfig();
        bulletConfig->behaviorConfig->behaviors.push_back(new BulletBehavior());
        EntityManager::GetInstance()->AddEntity(bulletConfig);
        
    }
}
