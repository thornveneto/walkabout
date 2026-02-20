#include "LineSegment.h"

Vector2D LineSegment::start_vector() const {
    return Vector2D(_x1, _y1);
};

Vector2D LineSegment::end_vector() const {
    return Vector2D(_x2, _y2);
};

bool LineSegment::intersects(const LineSegment& line_segment) const
{
    constexpr double EPS = 1e-6f;

    auto direction = [](
        double ax, double ay,
        double bx, double by,
        double cx, double cy
        ) {
            return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
        };


    auto sign = [&](double v) {
        //for tricky edge cases when multiplication is close 2 zero
        if (v > EPS) return  1;
        if (v < -EPS) return -1;
        return 0;
        };

    // Collinear / touching cases
    auto on_segment = [](double ax, double ay,
        double bx, double by,
        double cx, double cy
        ) {
            return cx >= std::min(ax, bx) && cx <= std::max(ax, bx) &&
                cy >= std::min(ay, by) && cy <= std::max(ay, by);
        };

    double d1 = direction(_x1, _y1, _x2, _y2, line_segment._x1, line_segment._y1);
    double d2 = direction(_x1, _y1, _x2, _y2, line_segment._x2, line_segment._y2);
    double d3 = direction(line_segment._x1, line_segment._y1, line_segment._x2, line_segment._y2, _x1, _y1);
    double d4 = direction(line_segment._x1, line_segment._y1, line_segment._x2, line_segment._y2, _x2, _y2);

    int s1 = sign(d1);
    int s2 = sign(d2);
    int s3 = sign(d3);
    int s4 = sign(d4);

    // General case
    if (s1 * s2 < 0 && s3 * s4 < 0) return true;

    if (s1 == 0 && on_segment(_x1, _y1, _x2, _y2, line_segment._x1, line_segment._y1)) return true;
    if (s2 == 0 && on_segment(_x1, _y1, _x2, _y2, line_segment._x2, line_segment._y2)) return true;
    if (s3 == 0 && on_segment(line_segment._x1, line_segment._y1, line_segment._x2, line_segment._y2, _x1, _y1)) return true;
    if (s4 == 0 && on_segment(line_segment._x1, line_segment._y1, line_segment._x2, line_segment._y2, _x2, _y2)) return true;

    return false;
}