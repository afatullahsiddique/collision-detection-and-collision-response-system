#pragma once
#include "Shape.h"

class Circle : public Shape {
public:
    float radius;

    Circle(float r) : radius(r) {}
    ShapeType getType() const override { return ShapeType::Circle; }
};
