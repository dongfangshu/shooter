#pragma once
#include <SDL2/SDL.h>
#include <vector>

class UpdateContext;

// 基础 UI 事件名
enum class UIEventName {
    OnClick,
    OnTouchBegin,
    OnTouchEnd,
    OnTouchUpdate,
    OnTouchLeave
};

// 冒泡时携带的事件数据
struct UIEvent {
    UIEventName name;
    int x{0};
    int y{0};
    uint32_t sdlType{0};  // 原始 SDL 事件类型，便于扩展
};

class UIComponent {
protected:
    SDL_Renderer* render{nullptr};
private:
    int width;
    int height;
    SDL_Point position;
    bool isVisible;
    bool isEnabled;
    UIComponent* parent{nullptr};
    std::vector<UIComponent*> children;
    
public:
    UIComponent(int width, int height);
    virtual ~UIComponent();
    virtual void SetUp();
    virtual void Update(UpdateContext* ctx);

    // 事件冒泡：从当前节点向上传递，返回 true 表示已处理并停止冒泡
    virtual bool BubbleEvent(UIEventName name, const UIEvent& ev);

    bool HitTest(int x, int y);
    // 层级命中：返回包含 (x,y) 的最深层子节点（含自身）
    virtual UIComponent* HitTestTarget(int x, int y);

    void AddChild(UIComponent* child);
    void RemoveChild(UIComponent* child);
    void RemoveAllChild();
    UIComponent* GetParent() const { return parent; }
    const std::vector<UIComponent*>& GetChildren() const { return children; }

    SDL_Rect GetRect() const;
    void SetPosition(int x, int y);
    void SetPosition(SDL_Point pos);
    SDL_Point GetPosition() const;
    SDL_Point GetWorldPosition() const;
    int GetWidth() const;
    void SetWidth(int width);
    int GetHeight() const;
    void SetHeight(int height);
    bool IsVisible() const;
    void SetVisible(bool visible);
    bool IsEnabled() const;
    void SetEnabled(bool enabled);
    void SetRender(SDL_Renderer* render);
};
