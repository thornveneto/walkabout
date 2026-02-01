#pragma once
#include <cmath>
#include <iostream>
struct Vector2D {
    double x, y;

    Vector2D() : x(0.0), y(0.0) {}

    Vector2D(double x, double y) :x(x), y(y) {}

    double length() {
        return sqrt(x * x + y * y);
    }

    double dot(const Vector2D& v) {
        return x * v.x + y * v.y;
    }

    void normalize() {
        double magnitude = length();
        x /= magnitude;
        y /= magnitude;
    }

    void truncate(double max_value) {
        if (length() > max_value) {
            normalize();

            *this *= max_value;
        }
    }

    double distance(const Vector2D& v) {
        double dx = v.x - x;
        double dy = v.y - y;

        return sqrt(dx * dx + dy * dy);
    }

    const Vector2D& operator+=(const Vector2D& v) {
        x += v.x;
        y += v.y;

        return *this;
    }

    const Vector2D& operator-=(const Vector2D& v) {
        x -= v.x;
        y -= v.y;

        return *this;
    }

    Vector2D& operator*=(const double scalar) {
        x *= scalar;
        y *= scalar;

        return *this;
    }

    friend Vector2D operator*(const Vector2D& lhs, double rhs) {
        Vector2D result(lhs);

        result *= rhs;

        return result;
    }

    friend Vector2D operator*(double lhs, const Vector2D& rhs) {
        Vector2D result(rhs);

        result *= lhs;

        return result;
    }

    friend Vector2D operator-(const Vector2D& lhs, const Vector2D& rhs) {
        Vector2D result(lhs);

        result -= rhs;

        return result;
    }

    //Vector2D pointAtRatio(const Vector2D& A, const Vector2D& B, float t) {
    //    return A * (1.0f - t) + B * t;
    //}
};