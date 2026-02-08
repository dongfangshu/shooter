#include "GButton.h"
#include "GText.h"
#include "../UpdateContext.h"
#include "UIConfig.h"

GButton::GButton(SDL_Rect rect)
	: UIComponent(rect), font(nullptr), onClick(nullptr) {
    normalColor = buttonNormalColor;
    hoverColor = buttonHoverColor;
    pressedColor = buttonPressedColor;
    currentColor = normalColor;
}

GButton::~GButton() {
    label = nullptr;  // 由基类析构统一删除 children
}

void GButton::SetUp() {
    label = new GText("");
	AddChild(label);
    SDL_Rect buttonRect = GetRect();
    label->SetPosition(SDL_Point{(buttonRect.w - label->GetRect().w) / 2, (buttonRect.h - label->GetRect().h) / 2});
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
    if (!IsVisible() || !ctx) return;
    // ctx->AddRenderCallback([this](SDL_Renderer* r) {
    //     SDL_Rect rect = GetRect();
    //     SDL_SetRenderDrawColor(r, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    //     SDL_RenderFillRect(r, &rect);
    // });
    for (auto* child : GetChildren()) {
        if (child->IsVisible())
            child->Update(ctx);
    }
}

void GButton::SetOnClick(std::function<void()> callback) {
    onClick = std::move(callback);
}

std::string GButton::GetText() const {
    return label ? label->GetText() : "";
}

void GButton::SetText( const std::string& newText) {
    if (label) label->SetText( newText);
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
