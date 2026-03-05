#pragma once
#include <SFML/Graphics.hpp>
#include "../math/Rectangle.h"
#include <functional>

struct UI_InputEvent;

class Button {
public:
	Button(std::string text_string, sf::Font& _font, Rectangle button_area);

	void handle_event(UI_InputEvent& event);

	void draw(sf::RenderWindow& window);

	void set_on_click_callback(std::function<void(const UI_InputEvent&)> callback);

private:
	bool is_pressed{ false };

	sf::Text text;
	std::string text_string;

	Rectangle button_area; //used to store geometry
	sf::RectangleShape button_shape;

	std::function<void(const UI_InputEvent&)> callback;
};
