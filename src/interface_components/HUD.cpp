#include "HUD.h"
#include <iostream>

HUD::HUD(std::deque<GameCommand>& command_queue) : just_button({ 100, 100, 100, 50 }), _command_queue{ command_queue } {
	std::cout << "Loading HUD resources:" << std::endl;

	std::cout << "Loading arial.ttf.....";
	if (_font.openFromFile("arial.ttf")) {
		std::cout << "SUCCESS" << std::endl;
	} else {
		std::cout << "FAILURE" << std::endl;
	}

	std::cout << "Loading soldier_red.png.....";
	if (_soldier_texture.loadFromFile("soldier_red.png")) {
		std::cout << "SUCCESS" << std::endl;
	} else {
		std::cout << "FAILURE" << std::endl;
	}

	just_button.on_click([&](const UI_InputEvent& event) {
		if (active_unit) {
			_command_queue.push_back(GameCommand(CommandType::MOVE, { 0,0 }, active_unit));
		}

	});
}

void HUD::set_data(Unit* unit) {
	if (unit) {
		active_unit = unit;
	} else {
		active_unit = nullptr;
	}
}

void HUD::draw_health_bar(sf::RenderWindow& window) {
	sf::Text health(_font);

	if (active_unit) {
		health.setString(std::to_string(active_unit->health()) + "/" + std::to_string(active_unit->max_health()));

		double health_ratio = static_cast<double>(active_unit->health()) / static_cast<double>(active_unit->max_health());

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
	if (active_unit) {
		sf::Sprite sprite(_soldier_texture);
		sprite.setPosition({ 400.f, 600.f });

		window.draw(sprite);
	}
}

void HUD::draw(sf::RenderWindow& window) {


	just_button.draw(window);

	draw_health_bar(window);
	draw_character_face(window);
}

void HUD::handle_event(UI_InputEvent& event) {
	just_button.handle_event(event);
}