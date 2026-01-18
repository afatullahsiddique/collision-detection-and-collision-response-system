#pragma once
#include "Shape.h"

class Rectangle : public Shape {
public:
    float width;
    float height;

    Rectangle(float w, float h) : width(w), height(h) {}
    ShapeType getType() const override { return ShapeType::Rectangle; }
};
