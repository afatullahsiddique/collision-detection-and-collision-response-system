#pragma once
#include "../math/Vec2.h"

enum class ShapeType { Circle, Rectangle, Polygon };

class Shape {
public:
    virtual ~Shape() = default;
    virtual ShapeType getType() const = 0;
};
