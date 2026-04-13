#pragma once
#include <SFML/Graphics.hpp>
#include "CellCube.h"
#include "CellAttackCube.h"
#include "../math/IJ.h"
#include "../GameWorld.h"
#include "../GameStateDesc.h"

class WorldRenderer;

class Cursor {
public:
	Cursor(GameWorld& game_world, float cur_cell_hw, float cur_cell_hh, float cur_cell_height);
	void draw(WorldRenderer& world_renderer, sf::RenderWindow& window, GameStateDesc& game_state_desc) const;

private:
	GameWorld& m_game_world;
	CellCube m_select_cube;
	CellAttackCube m_attack_cube;
	CellCube m_move_cube;

	void draw_select(WorldRenderer& world_renderer, sf::RenderWindow& window/*, sf::Vector2f cube_pos*/) const;
	void draw_attack(WorldRenderer& world_renderer, sf::RenderWindow& window/*, sf::Vector2f cube_pos*/) const;
	void draw_move(WorldRenderer& world_renderer, sf::RenderWindow& window/*, sf::Vector2f cube_pos*/) const;

	IJ temp_mouse_cell_ij(WorldRenderer& world_renderer, sf::RenderWindow& window) const;
};