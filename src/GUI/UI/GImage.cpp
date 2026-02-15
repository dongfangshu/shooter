#include "GImage.h"
#include "../UpdateContext.h"

GImage::GImage(SDL_Texture* texture) : UIComponent(0, 0), texture(texture) {
    if (texture) {
        int w = 0, h = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        SetWidth(w);
        SetHeight(h);
    }
}

GImage::~GImage() {
    // 不销毁 texture，由调用方（如 AssetManager）管理
}

void GImage::Update(UpdateContext* ctx) {
    if (!IsVisible() || !ctx || !texture) return;
    ctx->AddRenderCallback([this](SDL_Renderer* r) {
        SDL_Point worldPos = GetWorldPosition();
        SDL_Rect rect = {worldPos.x, worldPos.y, GetWidth(), GetHeight()};
        SDL_RenderCopy(r, texture, nullptr, &rect);
    });
}

void GImage::SetTexture(SDL_Texture* newTexture) {
    texture = newTexture;
    if (texture) {
        int w = 0, h = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        SetWidth(w);
        SetHeight(h);
    }
}

SDL_Texture* GImage::GetTexture() const {
    return texture;
}
