#pragma once
#include <SDL2/SDL.h>
#include <cmath>
#include <vector>

namespace Physics {

// 形状类型枚举
enum class ShapeType {
    AABB,       // 轴对齐包围盒
    Circle,     // 圆形
    OBB,        // 定向包围盒
    Polygon     // 多边形
};

// 基础形状类
class Shape {
public:
    virtual ~Shape() = default;
    virtual ShapeType GetType() const = 0;
    
    // 获取包围盒 (用于空间划分)
    virtual SDL_FRect GetBoundingBox() const = 0;
    
    // 点是否在形状内
    virtual bool Contains(float x, float y) const = 0;
    
    // 是否与另一个形状相交
    virtual bool Intersects(const Shape& other) const = 0;
    
    // 获取中心点
    virtual SDL_FPoint GetCenter() const = 0;
};

// AABB (轴对齐包围盒)
class AABB : public Shape {
private:
    float x, y;         // 左上角坐标
    float width, height;

public:
    AABB(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height) {}
    
    ShapeType GetType() const override { return ShapeType::AABB; }
    
    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetWidth() const { return width; }
    float GetHeight() const { return height; }
    float GetMinX() const { return x; }
    float GetMinY() const { return y; }
    float GetMaxX() const { return x + width; }
    float GetMaxY() const { return y + height; }
    
    void SetPosition(float newX, float newY) { x = newX; y = newY; }
    void SetSize(float newWidth, float newHeight) { width = newWidth; height = newHeight; }
    
    SDL_FRect GetBoundingBox() const override {
        return {x, y, width, height};
    }
    
    SDL_FPoint GetCenter() const override {
        return {x + width * 0.5f, y + height * 0.5f};
    }
    
    bool Contains(float px, float py) const override {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }
    
    bool Intersects(const Shape& other) const override;
    
    // AABB 与 AABB 相交检测
    bool IntersectsAABB(const AABB& other) const {
        return x < other.x + other.width &&
               x + width > other.x &&
               y < other.y + other.height &&
               y + height > other.y;
    }
    
    // AABB 与 Circle 相交检测
    bool IntersectsCircle(const class Circle& circle) const;
};

// 圆形
class Circle : public Shape {
private:
    float centerX, centerY;
    float radius;

public:
    Circle(float x, float y, float r)
        : centerX(x), centerY(y), radius(r) {}
    
    ShapeType GetType() const override { return ShapeType::Circle; }
    
    float GetCenterX() const { return centerX; }
    float GetCenterY() const { return centerY; }
    float GetRadius() const { return radius; }
    
    void SetCenter(float x, float y) { centerX = x; centerY = y; }
    void SetRadius(float r) { radius = r; }
    
    SDL_FRect GetBoundingBox() const override {
        return {centerX - radius, centerY - radius, radius * 2, radius * 2};
    }
    
    SDL_FPoint GetCenter() const override {
        return {centerX, centerY};
    }
    
    bool Contains(float px, float py) const override {
        float dx = px - centerX;
        float dy = py - centerY;
        return dx * dx + dy * dy <= radius * radius;
    }
    
    bool Intersects(const Shape& other) const override;
    
    // Circle 与 Circle 相交检测
    bool IntersectsCircle(const Circle& other) const {
        float dx = centerX - other.centerX;
        float dy = centerY - other.centerY;
        float distanceSq = dx * dx + dy * dy;
        float radiusSum = radius + other.radius;
        return distanceSq <= radiusSum * radiusSum;
    }
    
    // Circle 与 AABB 相交检测
    bool IntersectsAABB(const AABB& aabb) const;
};

// 射线 (用于射线检测)
struct Ray {
    SDL_FPoint origin;
    SDL_FPoint direction;  // 需要归一化
    float maxDistance;
    
    Ray(float x, float y, float dirX, float dirY, float maxDist = FLT_MAX)
        : origin{x, y}, direction{dirX, dirY}, maxDistance(maxDist) {
        // 归一化方向
        float len = std::sqrt(dirX * dirX + dirY * dirY);
        if (len > 0) {
            direction.x /= len;
            direction.y /= len;
        }
    }
};

// 射线检测结果
struct RaycastHit {
    bool hit;
    float distance;
    SDL_FPoint point;
    SDL_FPoint normal;
    int entityID;  // 击中的实体ID
    
    RaycastHit() : hit(false), distance(0), point{0, 0}, normal{0, 0}, entityID(-1) {}
};

} // namespace Physics
