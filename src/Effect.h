#pragma once
#include <SFML/Graphics.hpp>
class WorldRenderer;

class Effect {
public:
	virtual ~Effect();
	virtual bool is_alive() const = 0;
	virtual void update(sf::Time& deltaTime) = 0;
	virtual void draw(WorldRenderer& world_renderer) = 0;
};