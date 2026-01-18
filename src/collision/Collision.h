#pragma once
#include "../physics/RigidBody.h"

struct Manifold {
    RigidBody* A;
    RigidBody* B;
    Vec2 normal;
    float penetration;
};

bool CheckCollision(Manifold& m);
void ResolveCollision(Manifold& m);
