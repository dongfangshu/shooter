#include "BulletBehavior.h"
#include "../Entity/EntityManager.h"
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
    position->SetX(currentPos.x + speed * forward.x);
    position->SetY(currentPos.y + speed * forward.y);
}
void BulletBehavior::OnCollisionEnter(EntityHandle other)
{
    EntityManager::GetInstance()->RemoveEntity(entity->GetInstanceID());
}