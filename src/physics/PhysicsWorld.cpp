#include "PhysicsWorld.h"
#include "../collision/Collision.h"

void PhysicsWorld::step(float dt) {
    for (auto* b : bodies) {
        b->force = b->force + gravity * b->mass;
        b->integrate(dt);
    }

    for (size_t i = 0; i < bodies.size(); i++) {
        for (size_t j = i + 1; j < bodies.size(); j++) {
            Manifold m{bodies[i], bodies[j]};
            if (CheckCollision(m))
                ResolveCollision(m);
        }
    }
}
