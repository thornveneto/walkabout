#include "Button.h"
#include <iostream>
#include "../UI_InputEvent.h"

Button::Button(std::string text_string, sf::Font& _font, Rectangle button_area) : text_string{ text_string }, text { _font }, button_area{ button_area } {
	button_shape.setPosition({ static_cast<float>(button_area.x), static_cast<float>(button_area.y) });
	button_shape.setSize({ static_cast<float>(button_area.width), static_cast<float>(button_area.height) });
	button_shape.setFillColor(is_pressed ? sf::Color::Blue : sf::Color::Magenta);
	button_shape.setOutlineThickness(-2.f);
	button_shape.setOutlineColor(sf::Color::Magenta);

	text.setString(text_string);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(24); // in pixels, not points!
	text.setPosition({ static_cast<float>(button_area.x), static_cast<float>(button_area.y) });
}

void Button::handle_event(UI_InputEvent& event) {
	if (event.left_key_pressed && button_area.within(event.mouse_position)) {
		is_pressed = true;

		if (callback) {
			callback(event);
		}
	}
	if (event.left_key_released && button_area.within(event.mouse_position)) {

		is_pressed = false;
	}
}

void Button::draw(sf::RenderWindow& window) {
	button_shape.setFillColor(is_pressed ? sf::Color::Blue : sf::Color::Magenta);

	window.draw(button_shape);
	window.draw(text);
}

void Button::set_on_click_callback(std::function<void(const UI_InputEvent&)> callback) {
	this->callback = callback;
}