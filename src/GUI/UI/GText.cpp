#include "GText.h"
#include "../UpdateContext.h"
#include "../TextUtil.h"
#include "UIConfig.h"
GText::GText(const std::string& text)
    : UIComponent(0, 0) {
        this->text = text;
    if (!text.empty()) {
        int w = 0, h = 0;
        TextUtil::MeasureText(GetFont(), text.c_str(), &w, &h);
        SetWidth(w);
        SetHeight(h);
    }
}

GText::~GText() {
    if (textTexture != nullptr) {
        SDL_DestroyTexture(textTexture);
    }
}

void GText::UpdateTexture() {
    if (textTexture != nullptr) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }
    if (!GetFont() || text.empty()) return;
    SDL_Surface* surface = TTF_RenderUTF8_Blended(GetFont(), text.c_str(), color);
    if (surface) {
        textTexture = SDL_CreateTextureFromSurface(render, surface);
        SDL_FreeSurface(surface);
    }
}

void GText::Update(UpdateContext* ctx) {
    if (!IsVisible()) return;
    if (isDirty && !text.empty()) {
        UpdateTexture();
        isDirty = false;
    }
    if (!textTexture) return;
    ctx->AddRenderCallback([this](SDL_Renderer* r) {
        SDL_Point worldPos = GetWorldPosition();
        SDL_Rect rect = {worldPos.x, worldPos.y, GetWidth(), GetHeight()};
        SDL_RenderCopy(r, textTexture, nullptr, &rect);
    });
}

std::string GText::GetText() const {
    return text;
}

void GText::SetText( const std::string& newText) {
    if (text != newText) {
        text = newText;
        isDirty = true;
        if (!text.empty()) {
            int w = 0, h = 0;
            TextUtil::MeasureText(GetFont(), text.c_str(), &w, &h);
            SetWidth(w);
            SetHeight(h);
        }
    }
}

void GText::SetFont(TTF_Font* newFont) {
    if (font != newFont) {
        font = newFont;
        isDirty = true;
    }
}

void GText::SetColor( SDL_Color newColor) {
    if (color.r != newColor.r || color.g != newColor.g || color.b != newColor.b || color.a != newColor.a) {
        color = newColor;
        isDirty = true;
    }
}
TTF_Font* GText::GetFont() {
    if (font != nullptr)
    {
        return font;
    }
    return defaultFont;
}