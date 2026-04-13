#include "Cursor.h"
#include "../WorldRenderer.h"

Cursor::Cursor(GameWorld& game_world, float cur_cell_hw, float cur_cell_hh, float cur_cell_height) :
	m_game_world{ game_world },
	m_select_cube{ cur_cell_hw, cur_cell_hh, cur_cell_height, sf::Color::Cyan },
	m_attack_cube{ cur_cell_hw, cur_cell_hh, cur_cell_height, sf::Color::Red },
	m_move_cube{ cur_cell_hw, cur_cell_hh, cur_cell_height, sf::Color::Green }
{}

void Cursor::draw_select(WorldRenderer& world_renderer, sf::RenderWindow& window/*, sf::Vector2f cube_pos*/) const {
	IJ mouse_cell_ij = temp_mouse_cell_ij(world_renderer, window);

	if (m_game_world.terrain.within_boundaries(mouse_cell_ij)) {
		const XY<float> tile_xy = world_renderer.tile_screen_xy(mouse_cell_ij);

		m_select_cube.draw(world_renderer, sf::Vector2f{ tile_xy.x, tile_xy.y });
	}

}

void Cursor::draw_attack(WorldRenderer& world_renderer, sf::RenderWindow& window/*, sf::Vector2f cube_pos*/) const {
	IJ mouse_cell_ij = temp_mouse_cell_ij(world_renderer, window);

	if (m_game_world.terrain.within_boundaries(mouse_cell_ij)) {
		const XY<float> tile_xy = world_renderer.tile_screen_xy(mouse_cell_ij);

		m_attack_cube.draw(world_renderer, sf::Vector2f{ tile_xy.x, tile_xy.y });
	}
}

void Cursor::draw_move(WorldRenderer& world_renderer, sf::RenderWindow& window/*, sf::Vector2f cube_pos*/) const {
	IJ mouse_cell_ij = temp_mouse_cell_ij(world_renderer, window);

	if (m_game_world.terrain.within_boundaries(mouse_cell_ij)) {
		const XY<float> tile_xy = world_renderer.tile_screen_xy(mouse_cell_ij);

		m_move_cube.draw(world_renderer, sf::Vector2f{ tile_xy.x, tile_xy.y });
	}
}

void Cursor::draw(WorldRenderer& world_renderer, sf::RenderWindow& window, GameStateDesc& game_state_desc) const {
	if (game_state_desc.active_unit) {
		Weapon* active_weapon = game_state_desc.active_unit->active_weapon();

		if (active_weapon) {
			draw_attack(world_renderer, window);
		}
		else {
			draw_move(world_renderer, window);
		}
	}
	else {
		draw_select(world_renderer, window);//, sf::Vector2f{ tile_xy.x, tile_xy.y }/*TODO: dependency*/);
	}
}

IJ Cursor::temp_mouse_cell_ij(WorldRenderer& world_renderer, sf::RenderWindow& window) const {
	sf::Vector2i mouse_position = sf::Mouse::getPosition(window);

	//TODO: move to event data. it's needed in a lot of places
	auto mouse_cell_ij = world_renderer.tile_ij_from_screen_xy({ mouse_position.x, mouse_position.y });

	return mouse_cell_ij;
}