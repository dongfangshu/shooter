#include "MoveComponent.h"
#include "SDL2/SDL.h"
#include "Entity.h"
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
    position->Move(increment.x * speed,increment.y * speed);
    increment.x = 0;
    increment.y = 0;
}