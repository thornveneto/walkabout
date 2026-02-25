#include "HUD.h"
#include <iostream>
#include "../entities/Weapon.h"

HUD::HUD(std::deque<GameCommand>& command_queue) : 
	_btn_activate_main_weapon({ 100, 100, 100, 50 }), _btn_activate_aux_weapon({ 300, 100, 100, 50 }), _command_queue{ command_queue } {
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

	_btn_activate_main_weapon.set_on_click_callback([&](const UI_InputEvent& event) {
		if (active_unit) {
			std::cout << "clicked activate main weapon" << std::endl;

			//_command_queue.push_back(GameCommand(CommandType::MOVE, { 5,5 }, active_unit));
			_command_queue.push_back(GameCommand(CommandType::ACTIVATE_MAIN_WEAPON, { 0,0 }, active_unit));
		}

	});

	_btn_activate_aux_weapon.set_on_click_callback([&](const UI_InputEvent& event) {
		if (active_unit) {
			std::cout << "clicked activate aux weapon" << std::endl;

			_command_queue.push_back(GameCommand(CommandType::ACTIVATE_AUX_WEAPON, { 0,0 }, active_unit));
		}

		});
}

void HUD::set_active_unit(Unit* unit) {
	if (unit) {
		active_unit = unit;
	} else {
		active_unit = nullptr;
	}
}

void HUD::draw_weapon_status(sf::RenderWindow& window) {
	sf::Text weapon_status(_font);


	if (active_unit) {
		Weapon* active_weapon = active_unit->active_weapon();

		if (active_weapon && active_weapon->is_melee()) {
			weapon_status.setString("melee");
			weapon_status.setFillColor(sf::Color::Red);
		}
		else if (active_weapon && !active_weapon->is_melee()) {
			weapon_status.setString("ranged");
			weapon_status.setFillColor(sf::Color::Green);
		}
		else {
			weapon_status.setString("n/a");
			weapon_status.setFillColor(sf::Color::Blue);
		}
		
	}
	else {
		weapon_status.setString("n/a");
		weapon_status.setFillColor(sf::Color::Blue);
	}

	weapon_status.setCharacterSize(24); // in pixels, not points!



	weapon_status.setPosition({ 300.f, 700.f });

	window.draw(weapon_status);
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


	_btn_activate_main_weapon.draw(window);
	_btn_activate_aux_weapon.draw(window);

	draw_health_bar(window);
	draw_character_face(window);
	draw_weapon_status(window);
}

void HUD::handle_event(UI_InputEvent& event) {
	//TODO: not good that we need to copy this
	_btn_activate_main_weapon.handle_event(event);
	_btn_activate_aux_weapon.handle_event(event);
}