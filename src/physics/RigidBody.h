#pragma once
#include "../math/Vec2.h"
#include "../shapes/Shape.h"
#include <memory>

class RigidBody {
public:
    Vec2 position;
    Vec2 velocity;
    Vec2 force;
    float mass;
    float restitution;

    std::shared_ptr<Shape> shape;

    RigidBody(std::shared_ptr<Shape> s, float m)
        : shape(s), mass(m), restitution(0.8f) {}

    void integrate(float dt) {
        Vec2 acceleration = force * (1.0f / mass);
        velocity = velocity + acceleration * dt;
        position = position + velocity * dt;
        force = Vec2(0,0);
    }
};
