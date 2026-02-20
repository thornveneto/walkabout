#pragma once
#include <SFML/Graphics.hpp>
#include "../UI_InputEvent.h"
#include "../math/Rectangle.h"

class Button {
public:
	Button(Rectangle button_area) : button_area{ button_area } {
		button_shape.setPosition({ static_cast<float>(button_area.x), static_cast<float>(button_area.y)});
		button_shape.setSize({ static_cast<float>(button_area.width), static_cast<float>(button_area.height) });
		button_shape.setFillColor(sf::Color::Magenta);
	}

	void handle_event(UI_InputEvent& event) {
		if (event.left_key_pressed && button_area.within(event.mouse_position)) {
			std::cout << "SORT OF CLICKED" << std::endl;
		}
		
	}

	void draw(sf::RenderWindow& window) {
		window.draw(button_shape);
	}
private:
	Rectangle button_area; //used to store geometry
	sf::RectangleShape button_shape;
};
