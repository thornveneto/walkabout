#include "../GameWorld.h"
#include "../WorldRenderer.h"
#include <SFML/Graphics.hpp>
#include "../math/Rectangle.h"

class PlayField {
public:
    PlayField(GameWorld& game_world, WorldRenderer& world_renderer);

	void draw();

private:
	GameWorld& m_game_world;
	WorldRenderer& m_world_renderer;
    Rectangle m_screen_area;
    sf::RectangleShape m_border_rectangle;
};