#pragma once
struct Effect {
	virtual ~Effect() = default;
	virtual bool is_alive() = 0;
	virtual void update(sf::Time& deltaTime) = 0;
	virtual void draw(WorldRenderer& world_renderer) = 0;
};