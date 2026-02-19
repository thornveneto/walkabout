#include "HUD.h"
#include <iostream>

HUD::HUD() {
	if (_font.openFromFile("arial.ttf")) {
		std::cout << "Loaded font" << std::endl;
	} else {
		std::cout << "ERROR WHILE LOADING FONT" << std::endl;
	}

	if (_soldier_texture.loadFromFile("soldier_red.png")) {
		std::cout << "Loaded soldier picture" << std::endl;
	} else {
		std::cout << "ERROR WHILE LOADING soldier_red.png" << std::endl;
	}
}

void HUD::set_data(const Unit* unit) {
	if (unit) {
		_active_unit_health = unit->health();
		_active_unit_max_health = unit->max_health();
		_unit_selected = true;
	} else {
		_active_unit_health = 0;
		_active_unit_max_health = 0;
		_unit_selected = false;
	}
}

void HUD::draw_health_bar(sf::RenderWindow& window) {
	sf::Text health(_font);

	if (_unit_selected) {
		health.setString(std::to_string(_active_unit_health) + "/" + std::to_string(_active_unit_max_health));

		double health_ratio = static_cast<double>(_active_unit_health) / static_cast<double>(_active_unit_max_health);

		if (health_ratio < 0.25) {
			health.setFillColor(sf::Color::Red);
		}
		else if (health_ratio < 0.75) {
			health.setFillColor(sf::Color::Yellow);
		}
		else {
			health.setFillColor(sf::Color::Green);
		}

	} else {
		health.setString("-/-");
		health.setFillColor(sf::Color::Blue);
	}

	health.setCharacterSize(24); // in pixels, not points!



	health.setPosition({ 300.f, 600.f });

	window.draw(health);
}

void HUD::draw_character_face(sf::RenderWindow& window) {
	if (_unit_selected) {
		sf::Sprite sprite(_soldier_texture);
		sprite.setPosition({ 400.f, 600.f });

		window.draw(sprite);
	}
}

void HUD::draw(sf::RenderWindow& window) {

	draw_health_bar(window);
	draw_character_face(window);
}

