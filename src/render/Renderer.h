#pragma once
#include "../shapes/Shape.h"
#include <memory>
#include <GLFW/glfw3.h>
#include <vector>
#include <cstdint>

class Renderer {
private:
    int windowWidth, windowHeight;
    bool initialized;
    std::vector<uint8_t> frameBuffer;

public:
    Renderer();
    ~Renderer();
    
    void init(int width, int height);
    void clear();
    void beginFrame(int width, int height);
    void endFrame();
    void draw(std::shared_ptr<Shape> shape);
    void drawCircle(float x, float y, float radius, int colorIndex);
    void present();
    void saveFrameToPPM(const std::string& filename);
};
