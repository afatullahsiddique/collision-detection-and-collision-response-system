#include "Collision.h"
#include "../shapes/Circle.h"
#include "../shapes/Rectangle.h"

bool CircleCircle(Manifold& m) {
    auto* cA = static_cast<Circle*>(m.A->shape.get());
    auto* cB = static_cast<Circle*>(m.B->shape.get());

    Vec2 n = m.B->position - m.A->position;
    float dist = n.length();
    float r = cA->radius + cB->radius;

    if (dist >= r) return false;

    m.penetration = r - dist;
    m.normal = (dist == 0) ? Vec2(1, 0) : n.normalized();
    return true;
}

// Circle vs AABB (Axis-Aligned Bounding Box)
bool CircleRect(Manifold& m) {
    auto* circle = static_cast<Circle*>(m.A->shape.get());
    auto* rect = static_cast<Rectangle*>(m.B->shape.get());

    // Find closest point on rectangle to circle center
    float closestX = m.A->position.x;
    float closestY = m.A->position.y;
    
    float rectLeft = m.B->position.x - rect->getHalfWidth();
    float rectRight = m.B->position.x + rect->getHalfWidth();
    float rectTop = m.B->position.y - rect->getHalfHeight();
    float rectBottom = m.B->position.y + rect->getHalfHeight();
    
    closestX = (closestX < rectLeft) ? rectLeft : (closestX > rectRight) ? rectRight : closestX;
    closestY = (closestY < rectTop) ? rectTop : (closestY > rectBottom) ? rectBottom : closestY;
    
    float dx = m.A->position.x - closestX;
    float dy = m.A->position.y - closestY;
    float dist = std::sqrt(dx*dx + dy*dy);
    
    if (dist >= circle->radius) return false;
    
    m.penetration = circle->radius - dist;
    if (dist == 0) {
        m.normal = Vec2(1, 0);
    } else {
        m.normal = Vec2(dx, dy).normalized();
    }
    return true;
}

// Rect vs Circle (reverse argument check)
bool RectCircle(Manifold& m) {
    auto* rect = static_cast<Rectangle*>(m.A->shape.get());
    auto* circle = static_cast<Circle*>(m.B->shape.get());

    // Swap bodies for calculation
    RigidBody* tempA = m.A;
    m.A = m.B;
    m.B = tempA;
    
    bool result = CircleRect(m);
    
    // Swap back and invert normal
    tempA = m.A;
    m.A = m.B;
    m.B = tempA;
    m.normal = m.normal * (-1.0f);
    
    return result;
}

// Rectangle vs Rectangle (AABB vs AABB)
bool RectRect(Manifold& m) {
    auto* rectA = static_cast<Rectangle*>(m.A->shape.get());
    auto* rectB = static_cast<Rectangle*>(m.B->shape.get());
    
    float aLeft = m.A->position.x - rectA->getHalfWidth();
    float aRight = m.A->position.x + rectA->getHalfWidth();
    float aTop = m.A->position.y - rectA->getHalfHeight();
    float aBottom = m.A->position.y + rectA->getHalfHeight();
    
    float bLeft = m.B->position.x - rectB->getHalfWidth();
    float bRight = m.B->position.x + rectB->getHalfWidth();
    float bTop = m.B->position.y - rectB->getHalfHeight();
    float bBottom = m.B->position.y + rectB->getHalfHeight();
    
    // Check for overlap
    if (aRight < bLeft || aLeft > bRight || aBottom < bTop || aTop > bBottom) {
        return false;
    }
    
    // Calculate overlap on each axis
    float overlapLeft = aRight - bLeft;
    float overlapRight = bRight - aLeft;
    float overlapTop = aBottom - bTop;
    float overlapBottom = bBottom - aTop;
    
    float minOverlap = std::min(std::min(overlapLeft, overlapRight), std::min(overlapTop, overlapBottom));
    
    if (minOverlap == overlapLeft) {
        m.normal = Vec2(-1, 0);
        m.penetration = overlapLeft;
    } else if (minOverlap == overlapRight) {
        m.normal = Vec2(1, 0);
        m.penetration = overlapRight;
    } else if (minOverlap == overlapTop) {
        m.normal = Vec2(0, -1);
        m.penetration = overlapTop;
    } else {
        m.normal = Vec2(0, 1);
        m.penetration = overlapBottom;
    }
    
    return true;
}

bool CheckCollision(Manifold& m) {
    ShapeType aType = m.A->shape->getType();
    ShapeType bType = m.B->shape->getType();
    
    // Check Circle-Circle collisions
    if (aType == ShapeType::Circle && bType == ShapeType::Circle) {
        return CircleCircle(m);
    }
    
    // Check Circle-Rectangle collisions
    if (aType == ShapeType::Circle && bType == ShapeType::Rectangle) {
        return CircleRect(m);
    }
    
    // Check Rectangle-Circle collisions
    if (aType == ShapeType::Rectangle && bType == ShapeType::Circle) {
        return RectCircle(m);
    }
    
    // Check Rectangle-Rectangle collisions
    if (aType == ShapeType::Rectangle && bType == ShapeType::Rectangle) {
        return RectRect(m);
    }
    
    // TODO: Implement Polygon collision detection using SAT
    
    return false;
}

void ResolveCollision(Manifold& m) {
    Vec2 rv = m.B->velocity - m.A->velocity;
    float velAlongNormal = rv.dot(m.normal);
    if (velAlongNormal > 0) return;

    float e = std::min(m.A->restitution, m.B->restitution);
    float j = -(1 + e) * velAlongNormal;
    j /= (1 / m.A->mass + 1 / m.B->mass);

    Vec2 impulse = m.normal * j;
    m.A->velocity = m.A->velocity - impulse * (1 / m.A->mass);
    m.B->velocity = m.B->velocity + impulse * (1 / m.B->mass);
}
