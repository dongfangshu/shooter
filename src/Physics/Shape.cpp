#include "Shape.h"

namespace Physics {

bool AABB::Intersects(const Shape& other) const {
    switch (other.GetType()) {
        case ShapeType::AABB:
            return IntersectsAABB(static_cast<const AABB&>(other));
        case ShapeType::Circle:
            return IntersectsCircle(static_cast<const Circle&>(other));
        default:
            return false;
    }
}

bool AABB::IntersectsCircle(const Circle& circle) const {
    // 找到圆心到AABB最近的点
    float closestX = std::max(x, std::min(circle.GetCenterX(), x + width));
    float closestY = std::max(y, std::min(circle.GetCenterY(), y + height));
    
    float dx = circle.GetCenterX() - closestX;
    float dy = circle.GetCenterY() - closestY;
    float distanceSq = dx * dx + dy * dy;
    
    return distanceSq <= circle.GetRadius() * circle.GetRadius();
}

bool Circle::Intersects(const Shape& other) const {
    switch (other.GetType()) {
        case ShapeType::AABB:
            return IntersectsAABB(static_cast<const AABB&>(other));
        case ShapeType::Circle:
            return IntersectsCircle(static_cast<const Circle&>(other));
        default:
            return false;
    }
}

bool Circle::IntersectsAABB(const AABB& aabb) const {
    return aabb.IntersectsCircle(*this);
}

} // namespace Physics
