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
    
    EntityConfig* playerConfig = new EntityConfig();
    
    playerConfig->positionConfig = new PositionConfig();
    playerConfig->positionConfig->x = SCREEN_WIDTH / 2;
    playerConfig->positionConfig->y = SCREEN_HEIGHT - 64.0f;
    
    playerConfig->movementConfig = new MovementConfig();
    playerConfig->movementConfig->speed = 5.0f;
    
    playerConfig->collisionConfig = new CollisionConfig();
    playerConfig->collisionConfig->width = 64.0f;
    playerConfig->collisionConfig->height = 64.0f;
    
    playerConfig->behaviorConfig = new BehaviorConfig();
    playerConfig->behaviorConfig->behaviors.push_back(new PlayerBehavior());

    playerConfig->renderConfig = new RenderConfig();
    playerConfig->renderConfig->renderOrder = 100;
    playerConfig->renderConfig->texturePath = "assets/image/SpaceShip.png";
    
    playerHandle = entityManager->AddEntity(playerConfig);
    
    // AddEntity 内部会删除 playerConfig，无需手动删除
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