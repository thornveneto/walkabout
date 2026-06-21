#include <SFML/Graphics.hpp>
#include "../math/XY.h"
#include "../WorldRenderer.h"


class TileDrawing {
public:
	TileDrawing(XY<float> tile_xy, const sf::Color& fill_color, const sf::Color& outline_color, WorldRenderer& world_renderer) :
		world_renderer{world_renderer}
	{
		tile.setPointCount(4);

		tile.setPoint(0, { tile_xy.x, tile_xy.y - world_renderer.hh });
		tile.setPoint(1, { tile_xy.x + world_renderer.hw, tile_xy.y });
		tile.setPoint(2, { tile_xy.x, tile_xy.y + world_renderer.hh });
		tile.setPoint(3, { tile_xy.x - world_renderer.hw, tile_xy.y });

		tile.setFillColor(fill_color);

		//outline - negative thickness to extrude inside
		tile.setOutlineThickness(-1.f);
		tile.setOutlineColor(outline_color);
	}

	// Move
	TileDrawing(TileDrawing&& other, WorldRenderer& world_renderer) noexcept : world_renderer( world_renderer ) {}

	TileDrawing& operator=(TileDrawing&& other) noexcept {
		if (this != &other) {
			tile = std::move(other.tile);
		}

		return *this;
	}

	// No Copying allowed
	TileDrawing(const TileDrawing&) = delete;      
	TileDrawing& operator=(const TileDrawing&) = delete;

	void draw(WorldRenderer& world_renderer) {
		world_renderer.draw(tile);
	}
private:
	sf::ConvexShape tile;
	WorldRenderer& world_renderer;
};