#include "Renderer.h"
#include "ImageWriter.h"
#include "../shapes/Circle.h"
#include "../shapes/Rectangle.h"
#include <iostream>
#include <cmath>
#include <cstring>

Renderer::Renderer() : windowWidth(1200), windowHeight(800), initialized(false) {
}

Renderer::~Renderer() {
}

void Renderer::init(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    
    // Allocate frame buffer (RGB)
    frameBuffer.resize(width * height * 3, 25);  // Dark gray background
    
    initialized = true;
    std::cout << "Renderer initialized (" << width << "x" << height << ")\n";
}

void Renderer::clear() {
    // Fill with dark gray background
    std::fill(frameBuffer.begin(), frameBuffer.end(), 25);
}

void Renderer::beginFrame(int width, int height) {
    clear();
}

void Renderer::endFrame() {
    // Frame is ready
}

void drawFilledCircle(std::vector<uint8_t>& buffer, int width, int height, float cx, float cy, float radius, uint8_t r, uint8_t g, uint8_t b) {
    int x0 = (int)(cx - radius);
    int y0 = (int)(cy - radius);
    int x1 = (int)(cx + radius);
    int y1 = (int)(cy + radius);
    
    x0 = std::max(0, x0);
    y0 = std::max(0, y0);
    x1 = std::min(width - 1, x1);
    y1 = std::min(height - 1, y1);
    
    float r2 = radius * radius;
    
    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            float dx = x - cx;
            float dy = y - cy;
            if (dx*dx + dy*dy <= r2) {
                int idx = (y * width + x) * 3;
                buffer[idx + 0] = r;
                buffer[idx + 1] = g;
                buffer[idx + 2] = b;
            }
        }
    }
}

void Renderer::drawCircle(float x, float y, float radius, int colorIndex) {
    if (!initialized || radius <= 0) return;
    
    // Colors for different circles
    uint8_t colors[3][3] = {
        {255, 77, 77},    // Red
        {77, 255, 77},    // Green
        {77, 77, 255}     // Blue
    };
    
    int idx = colorIndex % 3;
    drawFilledCircle(frameBuffer, windowWidth, windowHeight, x, y, radius,
                     colors[idx][0], colors[idx][1], colors[idx][2]);
    
    // Draw outline
    drawFilledCircle(frameBuffer, windowWidth, windowHeight, x, y, radius + 1.0f,
                     200, 200, 200);
    drawFilledCircle(frameBuffer, windowWidth, windowHeight, x, y, radius,
                     colors[idx][0], colors[idx][1], colors[idx][2]);
}

void Renderer::draw(std::shared_ptr<Shape> shape) {
    // Placeholder
}

void Renderer::present() {
    // No window to present to
}

void Renderer::saveFrameToPPM(const std::string& filename) {
    ImageWriter::writePPM(filename, windowWidth, windowHeight, frameBuffer);
}
