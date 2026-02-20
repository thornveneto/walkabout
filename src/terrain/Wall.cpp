#include "Wall.h"
void Wall::draw(int i, int j, WorldRenderer& world_renderer) {

	if (wall_orientation == WallOrientation::NORTH) {
		draw_north_wall(
			i, j,
			sf::Color::Yellow, sf::Color::Red,
            world_renderer
		);
	}
	else if (wall_orientation == WallOrientation::EAST) {
		draw_east_wall(
			i, j,
			sf::Color::Yellow, sf::Color::Red,
            world_renderer
		);
	}
	else if (wall_orientation == WallOrientation::SOUTH) {
		draw_south_wall(
			i, j,
			sf::Color::Yellow, sf::Color::Red,
            world_renderer
		);
	}
	else if (wall_orientation == WallOrientation::WEST) {
		draw_west_wall(
			i, j,
			sf::Color::Yellow, sf::Color::Red,
            world_renderer
		);
	}
}

void Wall::_draw_wall(
    int i, int j, const sf::Color& fill_color, const sf::Color& outline_color, const std::vector<std::pair<float, float>>& wall_vertices,
    WorldRenderer& world_renderer
    ) {

    const XY<float> xy = world_renderer.tile_screen_xy({ i,j });

    sf::ConvexShape wall_shape;

    wall_shape.setPointCount(wall_vertices.size());

    // define the points
    for (int i = 0; i < wall_vertices.size(); ++i) {
        wall_shape.setPoint(i, { xy.x + wall_vertices[i].first, xy.y + wall_vertices[i].second });
    }

    wall_shape.setFillColor(fill_color);

    //outline - negative thickness to extrude inside
    wall_shape.setOutlineThickness(-1.f);
    wall_shape.setOutlineColor(outline_color);

    world_renderer.draw(wall_shape);
}

void Wall::draw_north_wall(int i, int j, const sf::Color& fill_color, const sf::Color& outline_color, WorldRenderer& world_renderer) {

    _draw_wall(
        i, j,
        fill_color,
        outline_color,
        { 
            {-world_renderer.hw, 0.f},
            {-world_renderer.hw, -world_renderer.cell_height},
            {0.f, -world_renderer.hh - world_renderer.cell_height },
            { 0.f, -world_renderer.hh}
        },
        world_renderer
    );
}

void Wall::draw_east_wall(int i, int j, const sf::Color& fill_color, const sf::Color& outline_color, WorldRenderer& world_renderer) {

    _draw_wall(
        i, j,
        fill_color,
        outline_color,
        { 
            {0.f,-world_renderer.hh },
            {0.f,-world_renderer.hh - world_renderer.cell_height },
            { world_renderer.hw, -world_renderer.cell_height }, 
            { world_renderer.hw, 0.f}
        },
        world_renderer
    );
}

void Wall::draw_south_wall(int i, int j, const sf::Color& fill_color, const sf::Color& outline_color, WorldRenderer& world_renderer) {
    _draw_wall(
        i, j,
        fill_color,
        outline_color,
        { 
            {0.f,world_renderer.hh },
            { 0.f, world_renderer.hh - world_renderer.cell_height },
            { world_renderer.hw, -world_renderer.cell_height },
            { world_renderer.hw, 0.f }
        },
        world_renderer
    );
}

void Wall::draw_west_wall(int i, int j, const sf::Color& fill_color, const sf::Color& outline_color, WorldRenderer& world_renderer) {
    _draw_wall(
        i, j,
        fill_color,
        outline_color,
        {
            {-world_renderer.hw,0.f },
            { -world_renderer.hw, -world_renderer.cell_height },
            { 0.f, world_renderer.hh - world_renderer.cell_height },
            {0.f, world_renderer.hh }
        },
        world_renderer
    );
}