#pragma once
#include <algorithm>
class LineSegment {
public:
    float x1;
    float y1;
    float x2;
    float y2;
    LineSegment(float x1, float y1, float x2, float y2) : x1{ x1 }, y1{ y1 }, x2{ x2 }, y2{ y2 } {}

    bool intersects(const LineSegment& line_segment)
    {
        constexpr float EPS = 1e-6f;

        auto direction = [](
            float ax, float ay,
            float bx, float by,
            float cx, float cy
            ) {
                return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
            };


        auto sign = [&](float v) {
            //for tricky edge cases when multiplication is close 2 zero
            if (v > EPS) return  1;
            if (v < -EPS) return -1;
            return 0;
            };

        // Collinear / touching cases
        auto on_segment = [](float ax, float ay,
            float bx, float by,
            float cx, float cy
            ) {
                return cx >= std::min(ax, bx) && cx <= std::max(ax, bx) &&
                    cy >= std::min(ay, by) && cy <= std::max(ay, by);
            };

        float d1 = direction(x1, y1, x2, y2, line_segment.x1, line_segment.y1);
        float d2 = direction(x1, y1, x2, y2, line_segment.x2, line_segment.y2);
        float d3 = direction(line_segment.x1, line_segment.y1, line_segment.x2, line_segment.y2, x1, y1);
        float d4 = direction(line_segment.x1, line_segment.y1, line_segment.x2, line_segment.y2, x2, y2);

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