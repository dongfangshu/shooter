#include "Player.h"
#include "../Entity/EntityManager.h"
#include "../Entity/EntityConfig.h"
#include "../Entity/PositionComponent.h"
#include "../Entity/MoveComponent.h"
#include "../Entity/CollisionComponent.h"
#include "../Entity/BehaviorComponent.h"
#include "PlayerBehavior.h"
#include "../Core/ScreenConstants.h"

Player* Player::instance = nullptr;

Player::Player() : playerHandle({-1}) {
}

Player::~Player() {
}

Player* Player::GetInstance() {
    if (!instance) {
        instance = new Player();
    }
    return instance;
}

void Player::DestroyInstance() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

void Player::Init(SDL_Renderer* renderer) {
    EntityManager* entityManager = EntityManager::GetInstance();
    
    auto playerConfig = std::make_unique<EntityConfig>();
    
    playerConfig->positionConfig = std::make_unique<PositionConfig>();
    playerConfig->positionConfig->x = SCREEN_WIDTH / 2.0f;
    playerConfig->positionConfig->y = SCREEN_HEIGHT - 100.0f;
    playerConfig->positionConfig->angle = 0.0f;  // 贴图朝上，无需旋转
    
    playerConfig->movementConfig = std::make_unique<MovementConfig>();
    playerConfig->movementConfig->speed = 300.0f;  // 每秒 300 像素
    
    playerConfig->collisionConfig = std::make_unique<CollisionConfig>(64.0f, 64.0f, false, false);
    
    playerConfig->behaviorConfig = std::make_unique<BehaviorConfig>();
    playerConfig->behaviorConfig->behaviors.push_back(std::make_unique<PlayerBehavior>());

    playerConfig->renderConfig = std::make_unique<RenderConfig>();
    playerConfig->renderConfig->renderOrder = 100;
    playerConfig->renderConfig->texturePath = "assets/image/SpaceShip.png";
    
    playerHandle = entityManager->AddEntity(std::move(playerConfig));
}

EntityHandle Player::GetPlayerHandle() const {
    return playerHandle;
}

Entity* Player::GetPlayerEntity() const {
    EntityManager* entityManager = EntityManager::GetInstance();
    return entityManager->GetEntity(playerHandle.entityID);
}

void Player::Update() {
    // Player的更新逻辑在PlayerBehavior中处理
}