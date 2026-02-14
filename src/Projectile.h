#pragma once
#include "MovingEntity.h"
#include "IJ.h"

class WorldRenderer;
class GameWorld;

class Projectile : public MovingEntity {
	bool _is_pending_sweep{ false };

	const double _projectile_radius = 5.f;
public:
	Projectile(IJ at_cell, WorldRenderer& world_renderer, GameWorld& game_world);

	virtual ~Projectile();

	void draw(WorldRenderer& world_renderer) override;

	bool is_pending_sweep() const;

	void mark_for_sweep();
};