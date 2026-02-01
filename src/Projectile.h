#pragma once
#include "MovingEntity.h"
#include "GameWorld.h"

class Projectile : public MovingEntity {
public:
	Projectile(IJ at_tile, WorldRenderer& world_renderer, GameWorld& game_world) : MovingEntity(at_tile, world_renderer, game_world) {

	}

	void draw(WorldRenderer& world_renderer) override;
};