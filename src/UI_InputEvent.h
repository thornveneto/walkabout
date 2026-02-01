#pragma once
#include "XY.h"
struct UI_InputEvent {
	bool left_key_pressed{ false };
	bool right_key_pressed{ false };
	XY<int> mouse_position{};
};