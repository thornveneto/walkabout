#pragma once
#include "MovingEntity.h"
#include "GameWorld.h"

class Projectile : public MovingEntity {
public:
	Projectile(IJ at_cell, WorldRenderer& world_renderer, GameWorld& game_world) : MovingEntity(at_cell, world_renderer, game_world) {

	}

	void draw(WorldRenderer& world_renderer) override;
};