#pragma once
#include "UIComponent.h"
#include <SDL2/SDL.h>

class GImage : public UIComponent {
private:
    SDL_Texture* texture;  // 不拥有所有权，由调用方管理

public:
    explicit GImage(SDL_Texture* texture);
    ~GImage();

    void Update(UpdateContext* ctx) override;

    void SetTexture(SDL_Texture* newTexture);
    SDL_Texture* GetTexture() const;
};
