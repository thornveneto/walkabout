#pragma once
#include "WorldRenderer.h"

enum class WallOrientation {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

class Wall {

	void _draw_wall(int i, int j, const sf::Color& fill_color, const sf::Color& outline_color, const std::vector<std::pair<float, float>>& wall_vertices, WorldRenderer& world_renderer);

	void draw_north_wall(int i, int j, const sf::Color& fill_color, const sf::Color& outline_color, WorldRenderer& world_renderer);

	void draw_east_wall(int i, int j, const sf::Color& fill_color, const sf::Color& outline_color, WorldRenderer& world_renderer);

	void draw_south_wall(int i, int j, const sf::Color& fill_color, const sf::Color& outline_color, WorldRenderer& world_renderer);

	void draw_west_wall(int i, int j, const sf::Color& fill_color, const sf::Color& outline_color, WorldRenderer& world_renderer);

public:
	WallOrientation wall_orientation;
	Wall(WallOrientation wall_orientation) : wall_orientation{ wall_orientation } {

	}
	
	void draw(int i, int j, WorldRenderer& world_renderer);
};