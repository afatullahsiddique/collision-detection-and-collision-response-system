#pragma once
#include "RigidBody.h"
#include <vector>

class PhysicsWorld {
public:
    std::vector<RigidBody*> bodies;
    Vec2 gravity = {0, 0};  // Disabled for now to see horizontal collision better

    void step(float dt);
};
