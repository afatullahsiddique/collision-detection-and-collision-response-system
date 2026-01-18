#pragma once
#include "Shape.h"
#include "../math/Vec2.h"
#include <vector>

class Polygon : public Shape {
public:
    std::vector<Vec2> vertices;

    Polygon(const std::vector<Vec2>& verts) : vertices(verts) {}
    ShapeType getType() const override { return ShapeType::Polygon; }
};
