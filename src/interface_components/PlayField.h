#include "../GameWorld.h"
#include "../WorldRenderer.h"
#include <SFML/Graphics.hpp>
#include "../math/Rectangle.h"
#include "../UI_InputEvent.h"
#include <deque>
#include "../GameCommand.h"
#include "../GameStateDesc.h"
#include "Cursor.h"

class PlayField {
public:
    PlayField(GameWorld& game_world, WorldRenderer& world_renderer, std::deque<GameCommand>& command_queue);

	void draw(sf::RenderWindow& window, GameStateDesc& game_state_desc);

	void handle_event(UI_InputEvent& event, GameStateDesc& game_state_desc);

private:
	GameWorld& m_game_world;
	WorldRenderer& m_world_renderer;
	std::deque<GameCommand>& m_command_queue;
    Rectangle m_screen_area;
    sf::RectangleShape m_border_rectangle;
	Cursor m_cursor;
};