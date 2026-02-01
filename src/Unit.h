#pragma once
#include "MovingEntity.h"
#include "GameWorld.h"
#include "WorldRenderer.h"
#include "IJ.h"
class Unit : public MovingEntity {
public:
	Unit(IJ at_cell, WorldRenderer& world_renderer, GameWorld& game_world) : MovingEntity(at_cell, world_renderer, game_world) {

	}

	void draw(WorldRenderer& world_renderer) override;

	void shoot_at(IJ target_cell, WorldRenderer& world_renderer);
};