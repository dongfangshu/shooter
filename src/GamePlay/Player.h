#pragma once
#include "../Entity/Entity.h"

class SDL_Renderer;

class Player {
private:
    static Player* instance;
    EntityHandle playerHandle;

    Player();
    ~Player();

public:
    static Player* GetInstance();
    static void DestroyInstance();

    void Init(SDL_Renderer* renderer);
    void Update();
    EntityHandle GetPlayerHandle() const;
    Entity* GetPlayerEntity() const;

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
};