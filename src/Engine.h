#pragma once
#include "physics/PhysicsWorld.h"
#include "render/Renderer.h"
#include "collision/Collision.h"
#include "shapes/Circle.h"
#include <memory>
#include <GLFW/glfw3.h>

class Engine {
private:
    PhysicsWorld physicsWorld;
    Renderer renderer;
    GLFWwindow* window;
    float deltaTime;
    bool running;
    int frameCount;
    float simulationTime;
    static const int WINDOW_WIDTH = 1200;
    static const int WINDOW_HEIGHT = 800;

public:
    Engine();
    ~Engine();
    
    void run();
    void update();
    void render();
    void handleInput();
    void shutdown();
    void createTestScene();
    bool initWindow();
    void processInput();
};
