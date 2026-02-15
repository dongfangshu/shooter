#include "UIComponent.h"
#include "../UpdateContext.h"
#include <algorithm>
UIComponent::UIComponent(int width, int height) : width(width), height(height), position({0, 0}), isVisible(true), isEnabled(true) {
        SetUp();
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
    for (auto* child : children) {
        if (child->IsVisible())
            child->Update(ctx);
    }
}

bool UIComponent::HitTest(int x, int y) {
    return x >= position.x && x < position.x + width && y >= position.y && y < position.y + height;
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
    return {position.x, position.y, width, height};
}

void UIComponent::SetPosition(int x, int y) {
    position.x = x;
    position.y = y;
}

void UIComponent::SetPosition(SDL_Point pos) {
    position = pos;
}

SDL_Point UIComponent::GetPosition() const {
    return position;
}

SDL_Point UIComponent::GetWorldPosition() const {
    if (!parent) {
        return position;
    }
    SDL_Point parentPos = parent->GetWorldPosition();
    return {position.x + parentPos.x, position.y + parentPos.y};
}

int UIComponent::GetWidth() const {
    return width;
}

void UIComponent::SetWidth(int width) {
    this->width = width;
}

int UIComponent::GetHeight() const {
    return height;
}

void UIComponent::SetHeight(int height) {
    this->height = height;
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
void UIComponent::SetRender(SDL_Renderer *render)
{
    this->render = render;
    for (auto *child : children)
    {
        child->SetRender(render);
    }
}
void UIComponent::SetUp() {
}