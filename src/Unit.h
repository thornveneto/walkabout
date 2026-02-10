#pragma once
#include "MovingEntity.h"
#include "IJ.h"

class GameWorld;
class WorldRenderer;

class Unit : public MovingEntity {
	bool _is_selected{ false };
public:
	Unit(IJ at_cell, WorldRenderer& world_renderer, GameWorld& game_world);

	virtual ~Unit();

	void select();

	void deselect();

	void draw(WorldRenderer& world_renderer) override;

	void shoot_at(IJ target_cell, WorldRenderer& world_renderer);
};