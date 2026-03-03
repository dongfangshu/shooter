#include "MoveComponent.h"
#include "SDL2/SDL.h"
#include "Entity.h"
#include "../Core/Time.h"

SDL_FPoint increment;
void MoveComponent::DoMove(SDL_FPoint offset)
{
    increment.x += offset.x;
    increment.y += offset.y;
}
void MoveComponent::LateUpdate()
{
    auto entity = GetEntity();
    auto position = entity->GetComponent<PositionComponent>();
    
    // 使用 deltaTime 确保移动速度与帧率无关
    float dt = Time::GetInstance()->GetDeltaTime();
    position->Move(increment.x * speed * dt, increment.y * speed * dt);
    increment.x = 0;
    increment.y = 0;
}