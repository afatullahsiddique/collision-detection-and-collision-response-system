#pragma once
#include "RigidBody.h"
#include <vector>

class PhysicsWorld {
public:
    std::vector<RigidBody*> bodies;
    Vec2 gravity = {0, 98.0f};  // Gravity in pixels/sec^2 (downward)

    void step(float dt);
};
