#include "BulletBehavior.h"
#include "../Entity/EntityManager.h"
#include "../Core/ScreenConstants.h"
#include "../Core/Time.h"
#include "SDL2/SDL.h"

BulletBehavior::BulletBehavior(/* args */)
{
}

BulletBehavior::~BulletBehavior()
{
}
void BulletBehavior::Update()
{
    if (!entity)
        return;
    auto position = entity->GetComponent<PositionComponent>();
    auto move = entity->GetComponent<MoveComponent>();
    auto speed = move->GetSpeed();
    auto forward = position->GetLeft();
    auto currentPos = position->GetPosition();
    
    // 使用 deltaTime 确保移动速度与帧率无关
    float dt = Time::GetInstance()->GetDeltaTime();
    
    // 更新位置
    float newX = currentPos.x + speed * forward.x * dt;
    float newY = currentPos.y + speed * forward.y * dt;
    position->SetX(newX);
    position->SetY(newY);
    
    // 检查是否超出屏幕边界，超出则移除子弹
    if (newX < 0 || newX > SCREEN_WIDTH || newY < 0 || newY > SCREEN_HEIGHT)
    {
        EntityManager::GetInstance()->RemoveEntity(entity->GetInstanceID());
    }
}
void BulletBehavior::OnCollisionEnter(EntityHandle other)
{
    EntityManager::GetInstance()->RemoveEntity(entity->GetInstanceID());
}