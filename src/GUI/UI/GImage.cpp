#include "GImage.h"
#include "../UpdateContext.h"

GImage::GImage(SDL_Texture* texture) : UIComponent({0, 0, 0, 0}), texture(texture) {
    if (texture) {
        int w = 0, h = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        SetRect({0, 0, w, h});
    }
}

GImage::~GImage() {
    // 不销毁 texture，由调用方（如 AssetManager）管理
}

void GImage::Update(UpdateContext* ctx) {
    if (!IsVisible() || !ctx || !texture) return;
    ctx->AddRenderCallback([this](SDL_Renderer* r) {
        SDL_Rect rect = GetRect();
        SDL_RenderCopy(r, texture, nullptr, &rect);
    });
}

void GImage::SetTexture(SDL_Texture* newTexture) {
    texture = newTexture;
    if (texture) {
        int w = 0, h = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        SDL_Rect r = GetRect();
        r.w = w;
        r.h = h;
        SetRect(r);
    }
}

SDL_Texture* GImage::GetTexture() const {
    return texture;
}
