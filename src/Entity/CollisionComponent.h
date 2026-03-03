#pragma once
#include "Component.h"
#include "EntityConfig.h"
#include "../Physics/Shape.h"

class PositionComponent;

class CollisionComponent : public Component
{
private:
    float width;
    float height;
    bool isStatic;  // 是否为静态物体（不移动）
    bool isTrigger; // 是否为触发器（不产生物理响应，只检测）
    
public:
    CollisionComponent(CollisionConfig* config, Entity* entity);
    ~CollisionComponent();
    
    // 基本属性
    float GetWidth() const { return width; }
    float GetHeight() const { return height; }
    bool IsStatic() const { return isStatic; }
    bool IsTrigger() const { return isTrigger; }
    
    void SetStatic(bool value) { isStatic = value; }
    void SetTrigger(bool value) { isTrigger = value; }
    
    // 获取当前 AABB 包围盒
    Physics::AABB GetAABB() const;
    
    // 检查点是否在碰撞体内
    bool ContainsPoint(float x, float y) const;
    
    // 获取中心位置
    SDL_FPoint GetCenter() const;
    
    // 更新回调（当位置变化时调用）
    void OnPositionChanged();
};