#include "Button.h"
#include <iostream>
#include "../UI_InputEvent.h"
#include "../palette.h"

Button::Button(std::string text_string, sf::Font& _font, Rectangle button_area) 
	: text_string{ text_string }, 
	text{ _font }, button_area{ button_area }
{
	button_shape.setPosition({ static_cast<float>(button_area.x), static_cast<float>(button_area.y) });
	button_shape.setSize({ static_cast<float>(button_area.width), static_cast<float>(button_area.height) });
	button_shape.setFillColor(is_pressed ? palette::DARK_GRAY : palette::GRAY);
	button_shape.setOutlineThickness(-2.f);
	button_shape.setOutlineColor(palette::GRAY);

	text.setString(text_string);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(24); // in pixels, not points!
	text.setPosition({ static_cast<float>(button_area.x), static_cast<float>(button_area.y) });
}

bool Button::fires_on_event(UI_InputEvent& event) {
	if (event.left_key_pressed && button_area.within(event.mouse_position)) {
		is_pressed = true;

		return true;
	} else if (event.left_key_released /* && button_area.within(event.mouse_position)*/) {

		is_pressed = false;
	}

	return false;
}

void Button::draw(sf::RenderWindow& window) {
	button_shape.setFillColor(is_pressed ? palette::DARK_GRAY : palette::GRAY);

	window.draw(button_shape);
	window.draw(text);
}