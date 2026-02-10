#pragma once
#include "MovingEntity.h"
#include "IJ.h"

class WorldRenderer;
class GameWorld;

class Projectile : public MovingEntity {
public:
	Projectile(IJ at_cell, WorldRenderer& world_renderer, GameWorld& game_world);

	virtual ~Projectile();

	void draw(WorldRenderer& world_renderer) override;
};