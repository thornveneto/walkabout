#pragma once
#include <SFML/Graphics.hpp>
#include "../entities//Unit.h"
#include "Button.h"
#include "../UI_InputEvent.h"
#include "../GameCommand.h"
#include <deque>
#include "../GameStateDesc.h"

class HUD {
public:
	HUD(sf::Font& _font, std::deque<GameCommand>& _command_queue);

	void handle_event(UI_InputEvent& event);

	void draw(sf::RenderWindow& window, GameStateDesc& game_state_desc);

	//void set_active_unit(Unit* unit);

private:
	Rectangle m_screen_area;
	sf::RectangleShape m_border_rectangle;

	sf::Font& _font;
	sf::Texture _soldier_texture;
	std::deque<GameCommand>& _command_queue;

	//Unit* active_unit{};

	void draw_health_bar(sf::RenderWindow& window, GameStateDesc& game_state_desc);
	void draw_character_face(sf::RenderWindow& window, GameStateDesc& game_state_desc);
	void draw_weapon_status(sf::RenderWindow& window, GameStateDesc& game_state_desc);

	Button _btn_activate_main_weapon;

	Button _btn_activate_aux_weapon;

};