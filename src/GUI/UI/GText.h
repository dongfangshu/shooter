#pragma once
#include "UIComponent.h"
#include <SDL2/SDL_ttf.h>
#include <string>

class GText : public UIComponent {
private:
    bool isDirty{false};
    std::string text;
    TTF_Font* font{nullptr};
    SDL_Color color;
    SDL_Texture* textTexture{nullptr};
    void UpdateTexture();

public:
    GText(const std::string& text);
    ~GText();

    void Update(UpdateContext* ctx) override;

    std::string GetText() const;
    void SetText(const std::string& newText);
    void SetFont(TTF_Font* newFont);
    TTF_Font* GetFont();
    void SetColor(SDL_Color newColor);
};
