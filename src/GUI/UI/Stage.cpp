#include "Stage.h"
#include "../UpdateContext.h"
#include <SDL2/SDL.h>

Stage::Stage(SDL_Renderer* renderer, int width, int height)
    : UIComponent({0, 0, width, height}) {
    render = renderer;
}

Stage::~Stage() {
}

void Stage::HandleEvent(const SDL_Event& event) {
    if (!IsEnabled()) return;

    int x = 0, y = 0;
    UIEventName name = UIEventName::OnClick;
    bool hasEvent = false;

    switch (event.type) {
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT) {
            x = event.button.x;
            y = event.button.y;
            name = UIEventName::OnTouchBegin;
            hasEvent = true;
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_LEFT) {
            x = event.button.x;
            y = event.button.y;
            name = UIEventName::OnTouchEnd;
            hasEvent = true;
        }
        break;
    case SDL_MOUSEMOTION: {
        x = event.motion.x;
        y = event.motion.y;
        UIComponent* newTarget = HitTestTarget(x, y);
        if (newTarget != lastMotionTarget) {
            if (lastMotionTarget) {
                UIEvent leaveEv;
                leaveEv.name = UIEventName::OnTouchLeave;
                leaveEv.x = x;
                leaveEv.y = y;
                leaveEv.sdlType = event.type;
                for (UIComponent* p = lastMotionTarget; p != nullptr; p = p->GetParent())
                    p->BubbleEvent(leaveEv.name, leaveEv);
            }
            lastMotionTarget = newTarget;
        }
        name = UIEventName::OnTouchUpdate;
        hasEvent = true;
        break;
    }
    default:
        break;
    }

    if (!hasEvent) return;

    UIComponent* target = HitTestTarget(x, y);
    if (!target) return;

    UIEvent ev;
    ev.name = name;
    ev.x = x;
    ev.y = y;
    ev.sdlType = event.type;

    for (UIComponent* p = target; p != nullptr; p = p->GetParent()) {
        if (p->BubbleEvent(ev.name, ev))
            break;
    }

    if (name == UIEventName::OnTouchEnd) {
        ev.name = UIEventName::OnClick;
        for (UIComponent* p = target; p != nullptr; p = p->GetParent()) {
            if (p->BubbleEvent(ev.name, ev))
                break;
        }
    }
}

void Stage::InternalUpdate(const std::vector<SDL_Event>& events) {
    for (const auto& e : events) {
        HandleEvent(e);
    }

    UpdateContext ctx(render);
    ctx.Begin();
    Update(&ctx);
    ctx.End();
}

void Stage::Update(UpdateContext* ctx) {
    if (!IsVisible()) return;
    for (auto* child : GetChildren()) {
        if (child->IsVisible())
            child->Update(ctx);
    }
}
