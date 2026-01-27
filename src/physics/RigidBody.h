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
    float dragCoefficient;  // Drag force coefficient (higher = more drag)

    std::shared_ptr<Shape> shape;

    RigidBody(std::shared_ptr<Shape> s, float m)
        : shape(s), mass(m), restitution(0.8f), dragCoefficient(0.02f) {}

    void integrate(float dt) {
        // Apply drag force: F_drag = -dragCoefficient * velocity
        Vec2 dragForce = velocity * (-dragCoefficient);
        Vec2 totalForce = force + dragForce;
        
        Vec2 acceleration = totalForce * (1.0f / mass);
        velocity = velocity + acceleration * dt;
        position = position + velocity * dt;
        force = Vec2(0, 0);
    }
};
