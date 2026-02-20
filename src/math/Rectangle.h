#pragma once
#include "XY.h"
struct Rectangle {
	int x;
	int y;
	int width;
	int height;

	Rectangle(int x, int y, int width, int height) : x{ x}, y{ y }, width{ width }, height{ height } {}

	bool within(XY<int> check_xy) const {
		return within(check_xy.x, check_xy.y);
	}

	bool within(int check_x, int check_y) const {
		return x <= check_x && check_x <= x + width && y <= check_y && check_y <= y + height;
	}
};