#include "UIComponent.h"
#include "../UpdateContext.h"
#include <algorithm>
UIComponent::UIComponent(SDL_Rect rect) : rect(rect), isVisible(true), isEnabled(true) {
}

UIComponent::~UIComponent() {
    for (auto* child : children) {
        child->parent = nullptr;
        delete child;
    }
    children.clear();
}

bool UIComponent::BubbleEvent(UIEventName name, const UIEvent& ev) {
    (void)name;
    (void)ev;
    return false;
}

void UIComponent::Update(UpdateContext* ctx) {
    (void)ctx;
}

bool UIComponent::HitTest(int x, int y) {
    return x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h;
}

UIComponent* UIComponent::HitTestTarget(int x, int y) {
    if (!HitTest(x, y)) return nullptr;
    for (int i = (int)children.size() - 1; i >= 0; --i) {
        UIComponent* c = children[i]->HitTestTarget(x, y);
        if (c) return c;
    }
    return this;
}

void UIComponent::AddChild(UIComponent* child) {
    child->SetRender(render);
    if (!child || child->parent == this) return;
    if (child->parent) child->parent->RemoveChild(child);
    child->parent = this;
    children.push_back(child);
}

void UIComponent::RemoveChild(UIComponent* child) {
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        (*it)->parent = nullptr;
        children.erase(it);
    }
}

SDL_Rect UIComponent::GetRect() const {
    return rect;
}

void UIComponent::SetRect(SDL_Rect newRect) {
    rect = newRect;
}

bool UIComponent::IsVisible() const {
    return isVisible;
}

void UIComponent::SetVisible(bool visible) {
    isVisible = visible;
}

bool UIComponent::IsEnabled() const {
    return isEnabled;
}

void UIComponent::SetEnabled(bool enabled) {
    isEnabled = enabled;
}
void UIComponent::SetRender(SDL_Renderer* render) {
    this->render = render;
}