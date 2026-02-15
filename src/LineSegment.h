#pragma once
#include <algorithm>
#include "Vector2D.h"
class LineSegment {
    double _x1;
    double _y1;

    double _x2;
    double _y2;
public:
    LineSegment(double x1, double y1, double x2, double y2) : _x1{ x1 }, _y1{ y1 }, _x2{ x2 }, _y2{ y2 } {}

    Vector2D start_vector() const;

    Vector2D end_vector() const;

    bool intersects(const LineSegment& line_segment) const;
};