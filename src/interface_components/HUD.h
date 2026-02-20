#pragma once
#include <SFML/Graphics.hpp>
#include "../entities//Unit.h"

class HUD {
	sf::Font _font;
	sf::Texture _soldier_texture;

	int _active_unit_health{ 0 };
	int _active_unit_max_health{ 0 };
	bool _unit_selected{ false };

	void draw_health_bar(sf::RenderWindow& window);
	void draw_character_face(sf::RenderWindow& window);
public:
	HUD();

	void draw(sf::RenderWindow& window);

	void set_data(const Unit* unit);
};