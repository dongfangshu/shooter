#include "GButton.h"
#include "GText.h"
#include "../UpdateContext.h"
#include "UIConfig.h"

GButton::GButton(int width, int height)
	: UIComponent(width, height), font(nullptr), onClick(nullptr) {
    normalColor = buttonNormalColor;
    hoverColor = buttonHoverColor;
    pressedColor = buttonPressedColor;
    currentColor = normalColor;
    label = new GText("");
    AddChild(label);
    label->SetPosition(SDL_Point{(width - label->GetRect().w) / 2, (height - label->GetRect().h) / 2});
}

GButton::~GButton() {
    label = nullptr;  // 由基类析构统一删除 children
}

void GButton::SetUp() {


}

bool GButton::BubbleEvent(UIEventName name, const UIEvent& ev) {
    (void)ev;
    if (!IsEnabled()) return false;
    switch (name) {
    case UIEventName::OnTouchBegin:
        isPressed = true;
        currentColor = pressedColor;
        return true;
    case UIEventName::OnTouchEnd:
        isPressed = false;
        currentColor = isHovered ? hoverColor : normalColor;
        return true;
    case UIEventName::OnClick:
        if (onClick) onClick();
        return true;
    case UIEventName::OnTouchUpdate:
        isHovered = true;
        if (!isPressed) currentColor = hoverColor;
        return true;
    case UIEventName::OnTouchLeave:
        isHovered = false;
        if (!isPressed) currentColor = normalColor;
        return true;
    default:
        return false;
    }
}

void GButton::Update(UpdateContext* ctx) {
    if (!IsVisible()) return;
    ctx->AddRenderCallback([this](SDL_Renderer* r) {
        SDL_Point worldPos = GetWorldPosition();
        SDL_Rect rect = {worldPos.x, worldPos.y, GetWidth(), GetHeight()};
        SDL_SetRenderDrawColor(r, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
        SDL_RenderFillRect(r, &rect);
    });
    UIComponent::Update(ctx);
}

void GButton::SetOnClick(std::function<void()> callback) {
    onClick = std::move(callback);
}

std::string GButton::GetText() const {
    return label ? label->GetText() : "";
}

void GButton::SetText( const std::string& newText) {
    label->SetText( newText);
    label->SetPosition(SDL_Point{(GetWidth() - label->GetRect().w) / 2, (GetHeight() - label->GetRect().h) / 2});
}

SDL_Color GButton::GetNormalColor() const { 
    if (normalColor.a == 0 && normalColor.r == 0 && normalColor.g == 0 && normalColor.b == 0){
 return buttonNormalColor;
    }
    return normalColor; 
}
void GButton::SetNormalColor(SDL_Color c) { normalColor = c; if (!isPressed && !isHovered) currentColor = c; }
SDL_Color GButton::GetHoverColor() const { return hoverColor; }
void GButton::SetHoverColor(SDL_Color c) { hoverColor = c; if (isHovered && !isPressed) currentColor = c; }
SDL_Color GButton::GetPressedColor() const { return pressedColor; }
void GButton::SetPressedColor(SDL_Color c) { pressedColor = c; if (isPressed) currentColor = c; }
