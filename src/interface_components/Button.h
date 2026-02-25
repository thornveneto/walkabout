#pragma once
#include <SFML/Graphics.hpp>
#include "../math/Rectangle.h"
#include <functional>

struct UI_InputEvent;

class Button {
public:
	Button(Rectangle button_area);

	void handle_event(UI_InputEvent& event);

	void draw(sf::RenderWindow& window);

	void set_on_click_callback(std::function<void(const UI_InputEvent&)> callback);

private:
	Rectangle _button_area; //used to store geometry
	sf::RectangleShape _button_shape;

	std::function<void(const UI_InputEvent&)> _callback;
};
