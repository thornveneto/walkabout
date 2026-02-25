#include "Button.h"
#include <iostream>
#include "../UI_InputEvent.h"

Button::Button(Rectangle button_area) : _button_area{ button_area } {
	_button_shape.setPosition({ static_cast<float>(button_area.x), static_cast<float>(button_area.y) });
	_button_shape.setSize({ static_cast<float>(button_area.width), static_cast<float>(button_area.height) });
	_button_shape.setFillColor(sf::Color::Magenta);
}

void Button::handle_event(UI_InputEvent& event) {
	if (event.left_key_pressed && _button_area.within(event.mouse_position) && _callback) {
		std::cout << "SORT OF CLICKED" << std::endl;

		_callback(event);
	}

}

void Button::draw(sf::RenderWindow& window) {
	window.draw(_button_shape);
}

void Button::set_on_click_callback(std::function<void(const UI_InputEvent&)> callback) {
	_callback = callback;
}