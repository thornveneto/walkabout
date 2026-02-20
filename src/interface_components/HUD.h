#pragma once
#include <SFML/Graphics.hpp>
#include "../entities//Unit.h"
#include "Button.h"
#include "../UI_InputEvent.h"
#include "../GameCommand.h"
#include <deque>

class HUD {
	sf::Font _font;
	sf::Texture _soldier_texture;
	std::deque<GameCommand>& _command_queue;

	Unit* active_unit;

	void draw_health_bar(sf::RenderWindow& window);
	void draw_character_face(sf::RenderWindow& window);

	Button just_button;
public:
	HUD(std::deque<GameCommand>& _command_queue);

	void handle_event(UI_InputEvent& event);

	void draw(sf::RenderWindow& window);

	void set_data(Unit* unit);
};