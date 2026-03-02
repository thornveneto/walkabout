#include "Button.h"
#include <iostream>
#include "../UI_InputEvent.h"

Button::Button(std::string text_string, sf::Font& _font, Rectangle button_area) : _text_string{ text_string }, _text { _font }, _button_area{ button_area } {
	_button_shape.setPosition({ static_cast<float>(button_area.x), static_cast<float>(button_area.y) });
	_button_shape.setSize({ static_cast<float>(button_area.width), static_cast<float>(button_area.height) });
	_button_shape.setFillColor(_is_pressed ? sf::Color::Blue : sf::Color::Magenta);
	_button_shape.setOutlineThickness(-2.f);
	_button_shape.setOutlineColor(sf::Color::Magenta);

	_text.setString(text_string);
	_text.setFillColor(sf::Color::White);
	_text.setCharacterSize(24); // in pixels, not points!
	_text.setPosition({ static_cast<float>(button_area.x), static_cast<float>(button_area.y) });
}

void Button::handle_event(UI_InputEvent& event) {
	if (event.left_key_pressed && _button_area.within(event.mouse_position)) {

		_is_pressed = true;

		if (_callback) {
			_callback(event);
		}
	}
	if (event.left_key_released && _button_area.within(event.mouse_position)) {

		_is_pressed = false;
	}
}

void Button::draw(sf::RenderWindow& window) {
	_button_shape.setFillColor(_is_pressed ? sf::Color::Blue : sf::Color::Magenta);

	window.draw(_button_shape);
	window.draw(_text);
}

void Button::set_on_click_callback(std::function<void(const UI_InputEvent&)> callback) {
	_callback = callback;
}