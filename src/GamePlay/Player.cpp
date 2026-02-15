#include "Player.h"
#include "../Entity/EntityManager.h"
#include "../Entity/EntityConfig.h"
#include "../Entity/PositionComponent.h"
#include "../Entity/MovementComponent.h"
#include "../Entity/CollisionComponent.h"
#include "../Entity/BehaviorComponent.h"
#include "PlayerBehavior.h"

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
    playerConfig->positionConfig->x = 400.0f;
    playerConfig->positionConfig->y = 500.0f;
    
    playerConfig->movementConfig = new MovementConfig();
    playerConfig->movementConfig->speed = 5.0f;
    
    playerConfig->collisionConfig = new CollisionConfig();
    playerConfig->collisionConfig->width = 64.0f;
    playerConfig->collisionConfig->height = 64.0f;
    
    playerConfig->behaviorConfig = new BehaviorConfig();
    playerConfig->behaviorConfig->behaviorType = 1;
    
    playerHandle = entityManager->AddEntity(playerConfig);
    
    // 获取玩家实体并添加PlayerBehavior
    Entity* player = entityManager->GetEntity(playerHandle.entityID);
    if (player) {
        BehaviorComponent* behaviorComponent = player->GetComponent<BehaviorComponent>();
        if (behaviorComponent) {
            behaviorComponent->SetBehavior(new PlayerBehavior(player));
        }
    }
    
    delete playerConfig;
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