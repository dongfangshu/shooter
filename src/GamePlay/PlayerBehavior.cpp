#include "PlayerBehavior.h"
#include "../Entity/Entity.h"
#include "../Entity/PositionComponent.h"
#include "../Entity/MovementComponent.h"
#include "../Input/InputManager.h"

PlayerBehavior::PlayerBehavior(Entity* entity) : entity(entity), speed(5.0f) {
}

PlayerBehavior::~PlayerBehavior() {
}

void PlayerBehavior::Start() {
    // 获取MovementComponent中的速度
    MovementComponent* movement = entity->GetComponent<MovementComponent>();
    if (movement) {
        speed = movement->GetSpeed();
    }
}

void PlayerBehavior::Update() {
    InputManager* inputManager = InputManager::GetInstance();
    PositionComponent* position = entity->GetComponent<PositionComponent>();

    if (position) {
        // 检测键盘输入并移动玩家
        if (inputManager->IsKeyPressed(SDLK_w)) {
            position->Move(0, -speed);
        }
        if (inputManager->IsKeyPressed(SDLK_s)) {
            position->Move(0, speed);
        }
        if (inputManager->IsKeyPressed(SDLK_a)) {
            position->Move(-speed, 0);
        }
        if (inputManager->IsKeyPressed(SDLK_d)) {
            position->Move(speed, 0);
        }
    }
}

void PlayerBehavior::SetSpeed(float speed) {
    this->speed = speed;
}

float PlayerBehavior::GetSpeed() const {
    return speed;
}