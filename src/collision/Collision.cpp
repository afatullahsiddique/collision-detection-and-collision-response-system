#include "Collision.h"
#include "../shapes/Circle.h"

bool CircleCircle(Manifold& m) {
    auto* cA = static_cast<Circle*>(m.A->shape.get());
    auto* cB = static_cast<Circle*>(m.B->shape.get());

    Vec2 n = m.B->position - m.A->position;
    float dist = n.length();
    float r = cA->radius + cB->radius;

    if (dist >= r) return false;

    m.penetration = r - dist;
    m.normal = n.normalized();
    return true;
}

bool CheckCollision(Manifold& m) {
    ShapeType aType = m.A->shape->getType();
    ShapeType bType = m.B->shape->getType();
    
    // Check Circle-Circle collisions
    if (aType == ShapeType::Circle && bType == ShapeType::Circle) {
        return CircleCircle(m);
    }
    
    // TODO: Implement other collision types (Rectangle, Polygon, mixed types)
    
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
