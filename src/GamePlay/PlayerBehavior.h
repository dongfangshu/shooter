#pragma once
#include "../Entity/BaseBehavior.h"
#include "../Entity/PositionComponent.h"
#include "SDL2/SDL.h"

class PlayerBehavior : public BaseBehavior {
private:
    // 检查玩家是否可以向指定方向移动
    bool CanMoveInDirection(PositionComponent* position, SDL_FPoint direction,float width,float height);
    void CheckMove();
    void CheckFire();

public:
    void Start() override;
    void Update() override;
};