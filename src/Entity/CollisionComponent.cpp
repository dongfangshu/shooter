#include "CollisionComponent.h"
#include "PositionComponent.h"
#include "Entity.h"
#include "CollisionManager.h"

CollisionComponent::CollisionComponent(CollisionConfig* config, Entity* entity)
    : Component(entity), width(config->width), height(config->height),
      isStatic(config->isStatic), isTrigger(config->isTrigger) {
    CollisionManager::GetInstance()->AddCollisionComponent(this);
}

CollisionComponent::~CollisionComponent() {
    CollisionManager::GetInstance()->RemoveCollisionComponent(this);
}

Physics::AABB CollisionComponent::GetAABB() const {
    PositionComponent* pos = GetEntity()->GetComponent<PositionComponent>();
    if (pos) {
        return Physics::AABB(pos->GetX(), pos->GetY(), width, height);
    }
    return Physics::AABB(0, 0, width, height);
}

bool CollisionComponent::ContainsPoint(float x, float y) const {
    Physics::AABB aabb = GetAABB();
    return aabb.Contains(x, y);
}

SDL_FPoint CollisionComponent::GetCenter() const {
    PositionComponent* pos = GetEntity()->GetComponent<PositionComponent>();
    if (pos) {
        return {pos->GetX() + width * 0.5f, pos->GetY() + height * 0.5f};
    }
    return {width * 0.5f, height * 0.5f};
}

void CollisionComponent::OnPositionChanged() {
    // 通知 CollisionManager 更新空间哈希
    // 实际更新在 CollisionManager::UpdateSpatialHash 中统一处理
}
