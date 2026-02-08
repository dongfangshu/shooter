#pragma once
#include "UIComponent.h"
#include <functional>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class GText;

class GButton : public UIComponent {
private:
    TTF_Font* font;
    SDL_Color normalColor;
    SDL_Color hoverColor;
    SDL_Color pressedColor;
    SDL_Color currentColor;
    bool isPressed{false};
    bool isHovered{false};
    GText* label{nullptr};
    std::function<void()> onClick;

public:
    GButton(SDL_Rect rect);
    ~GButton();

    bool BubbleEvent(UIEventName name, const UIEvent& ev) override;
    void Update(UpdateContext* ctx) override;

    void SetOnClick(std::function<void()> callback);
    std::string GetText() const;
    void SetText( const std::string& newText);

    SDL_Color GetNormalColor() const;
    void SetNormalColor(SDL_Color c);
    SDL_Color GetHoverColor() const;
    void SetHoverColor(SDL_Color c);
    SDL_Color GetPressedColor() const;
    void SetPressedColor(SDL_Color c);
};
