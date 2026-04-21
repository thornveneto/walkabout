#pragma once
#include "math/XY.h"
struct UI_InputEvent {
	bool left_key_pressed{ false };
	bool left_key_released{ false };

	bool right_key_pressed{ false };
	bool right_key_released{ false };

	XY<int> mouse_position{};
	bool close_window_request{ false };
};