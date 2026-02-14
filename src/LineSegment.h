#pragma once
#include <algorithm>
class LineSegment {
public:
    double x1;
    double y1;
    double x2;
    double y2;
    LineSegment(double x1, double y1, double x2, double y2) : x1{ x1 }, y1{ y1 }, x2{ x2 }, y2{ y2 } {}

    bool intersects(const LineSegment& line_segment)
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

        double d1 = direction(x1, y1, x2, y2, line_segment.x1, line_segment.y1);
        double d2 = direction(x1, y1, x2, y2, line_segment.x2, line_segment.y2);
        double d3 = direction(line_segment.x1, line_segment.y1, line_segment.x2, line_segment.y2, x1, y1);
        double d4 = direction(line_segment.x1, line_segment.y1, line_segment.x2, line_segment.y2, x2, y2);

        int s1 = sign(d1);
        int s2 = sign(d2);
        int s3 = sign(d3);
        int s4 = sign(d4);

        // General case
        if (s1 * s2 < 0 && s3 * s4 < 0) return true;

        if (s1 == 0 && on_segment(x1, y1, x2, y2, line_segment.x1, line_segment.y1)) return true;
        if (s2 == 0 && on_segment(x1, y1, x2, y2, line_segment.x2, line_segment.y2)) return true;
        if (s3 == 0 && on_segment(line_segment.x1, line_segment.y1, line_segment.x2, line_segment.y2, x1, y1)) return true;
        if (s4 == 0 && on_segment(line_segment.x1, line_segment.y1, line_segment.x2, line_segment.y2, x2, y2)) return true;

        return false;
    }
};