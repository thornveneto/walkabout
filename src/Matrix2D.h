#pragma once
#include "Vector2D.h"

struct Matrix2D {
	float a, b; //top row
	float c, d; //bottom row

	Matrix2D(float a, float b, float c, float d) : a{ a }, b{ b }, c{ c }, d{ d } {}

	Vector2D operator*(const Vector2D& v) const {
		return {
			a*v.x + b*v.y,
			c*v.x + d*v.y
		};
	}
};