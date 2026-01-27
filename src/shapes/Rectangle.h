#pragma once
#include "Shape.h"

class Rectangle : public Shape {
public:
    float width;   // Width in pixels
    float height;  // Height in pixels

    Rectangle(float w, float h) : width(w), height(h) {}
    ShapeType getType() const override { return ShapeType::Rectangle; }
    
    // Get half-width and half-height for AABB calculations
    float getHalfWidth() const { return width / 2.0f; }
    float getHalfHeight() const { return height / 2.0f; }
};
