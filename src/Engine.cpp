#include "Engine.h"
#include "render/Renderer.h"
#include <iostream>

Engine::Engine() : window(nullptr), deltaTime(0.016f), running(true), frameCount(0), simulationTime(0.0f) {
}

Engine::~Engine() {
}

bool Engine::initWindow() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    // Try headless mode first
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Collision Detection System", nullptr, nullptr);
    if (!window) {
        std::cerr << "Warning: Could not create GLFW window. Running in simulation-only mode.\n";
        // Continue without window - we'll still run physics simulation
        return true;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    if (glfwGetCurrentContext()) {
        glfwShowWindow(window);
        std::cout << "GLFW window created successfully\n";
    }
    
    return true;
}

void Engine::createTestScene() {
    // Create some test objects with larger sizes
    auto circle1 = std::make_shared<Circle>(30.0f);  // radius 30
    auto body1 = new RigidBody(circle1, 1.0f);
    body1->position = Vec2(300, 100);  // position in pixels
    body1->velocity = Vec2(100, 0);    // velocity in pixels/sec
    physicsWorld.bodies.push_back(body1);

    auto circle2 = std::make_shared<Circle>(30.0f);  // radius 30
    auto body2 = new RigidBody(circle2, 1.0f);
    body2->position = Vec2(900, 100);  // position in pixels
    body2->velocity = Vec2(-100, 0);   // velocity in pixels/sec
    physicsWorld.bodies.push_back(body2);

    std::cout << "Test scene created with " << physicsWorld.bodies.size() << " bodies\n";
}

void Engine::run() {
    if (!initWindow()) {
        return;
    }

    renderer.init(WINDOW_WIDTH, WINDOW_HEIGHT);
    createTestScene();
    
    std::cout << "Engine started\n";
    std::cout << "Running physics simulation...\n";
    
    // Run simulation loop
    while (running) {
        // Stop after 10 seconds or if window is closed
        if (window && glfwWindowShouldClose(window)) break;
        
        processInput();
        update();
        render();  // Always render, even in headless mode
        
        if (window && glfwGetCurrentContext()) {
            glfwSwapBuffers(window);
        }
        
        glfwPollEvents();
        
        // Stop after 10 seconds of simulation
        if (simulationTime > 10.0f) {
            running = false;
        }
    }
    
    shutdown();
}

void Engine::update() {
    // Update physics world
    physicsWorld.step(deltaTime);
    
    // Check and resolve collisions
    for (size_t i = 0; i < physicsWorld.bodies.size(); ++i) {
        for (size_t j = i + 1; j < physicsWorld.bodies.size(); ++j) {
            Manifold m;
            m.A = physicsWorld.bodies[i];
            m.B = physicsWorld.bodies[j];
            
            if (CheckCollision(m)) {
                ResolveCollision(m);
                std::cout << "Collision detected between body " << i << " and " << j << "\n";
            }
        }
    }
    
    // Check window boundaries
    checkBounds();
    
    frameCount++;
    simulationTime += deltaTime;
    
    // Print status every 30 frames
    if (frameCount % 30 == 0) {
        std::cout << "Frame " << frameCount << ", Time: " << simulationTime << "s\n";
    }
}

void Engine::render() {
    renderer.clear();
    renderer.beginFrame(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // Render all bodies
    for (size_t i = 0; i < physicsWorld.bodies.size(); ++i) {
        RigidBody* body = physicsWorld.bodies[i];
        Circle* circle = nullptr;
        
        if (body->shape->getType() == ShapeType::Circle) {
            circle = static_cast<Circle*>(body->shape.get());
            renderer.drawCircle(body->position.x, body->position.y, circle->radius, i);
        }
    }
    
    renderer.endFrame();
    
    // Save every 30 frames to file
    if (frameCount % 30 == 0) {
        char filename[256];
        snprintf(filename, sizeof(filename), "/tmp/collision_frame_%04d.ppm", frameCount);
        renderer.saveFrameToPPM(filename);
        std::cout << "Saved: " << filename << "\n";
    }
}

void Engine::processInput() {
    if (!window) return;
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        running = false;
    }
}

void Engine::handleInput() {
    // TODO: Handle other input events
}

void Engine::shutdown() {
    std::cout << "Engine shutting down after " << frameCount << " frames\n";
    
    // Cleanup
    for (auto body : physicsWorld.bodies) {
        delete body;
    }
    physicsWorld.bodies.clear();
    
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

void Engine::checkBounds() {
    // Boundary constants (window is 1200x800)
    const float BOUNDARY_LEFT = 0.0f;
    const float BOUNDARY_RIGHT = WINDOW_WIDTH;
    const float BOUNDARY_TOP = 0.0f;
    const float BOUNDARY_BOTTOM = WINDOW_HEIGHT;
    const float BOUNCE_DAMPING = 0.8f;  // Energy loss on bounce
    
    for (RigidBody* body : physicsWorld.bodies) {
        if (!body) continue;
        
        Circle* circle = nullptr;
        if (body->shape->getType() == ShapeType::Circle) {
            circle = static_cast<Circle*>(body->shape.get());
        }
        
        if (!circle) continue;
        
        float radius = circle->radius;
        
        // Check left boundary
        if (body->position.x - radius < BOUNDARY_LEFT) {
            body->position.x = BOUNDARY_LEFT + radius;
            body->velocity.x *= -BOUNCE_DAMPING;
            std::cout << "Body " << (body - physicsWorld.bodies[0]) << " bounced off LEFT boundary\n";
        }
        
        // Check right boundary
        if (body->position.x + radius > BOUNDARY_RIGHT) {
            body->position.x = BOUNDARY_RIGHT - radius;
            body->velocity.x *= -BOUNCE_DAMPING;
            std::cout << "Body " << (body - physicsWorld.bodies[0]) << " bounced off RIGHT boundary\n";
        }
        
        // Check top boundary
        if (body->position.y - radius < BOUNDARY_TOP) {
            body->position.y = BOUNDARY_TOP + radius;
            body->velocity.y *= -BOUNCE_DAMPING;
            std::cout << "Body " << (body - physicsWorld.bodies[0]) << " bounced off TOP boundary\n";
        }
        
        // Check bottom boundary
        if (body->position.y + radius > BOUNDARY_BOTTOM) {
            body->position.y = BOUNDARY_BOTTOM - radius;
            body->velocity.y *= -BOUNCE_DAMPING;
            std::cout << "Body " << (body - physicsWorld.bodies[0]) << " bounced off BOTTOM boundary\n";
        }
    }
}
