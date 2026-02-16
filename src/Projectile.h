#pragma once
#include "MovingEntity.h"
#include "IJ.h"
#include "ICollideable.h"
#include "_types.h"

class WorldRenderer;
class GameWorld;

class Projectile : public MovingEntity, public ICollideable {
	bool _is_pending_sweep{ false };

	const double _projectile_radius = 5.f;

	const int _damage_power = 50;//TODO: random value for now. Make projectile type dependent

	IdType _owner_id{ -1 }; //This is to enable render projectile over shooter, without colliding with the shooter.
public:
	Projectile(IJ at_cell, WorldRenderer& world_renderer, GameWorld& game_world, IdType owner_id);

	virtual ~Projectile();

	void draw(WorldRenderer& world_renderer) override;

	bool is_pending_sweep() const;

	void mark_for_sweep();

	void on_collision(const CollisionData& collision_data) override;

	IdType owner_id() const;

	int damage_power() const;
};