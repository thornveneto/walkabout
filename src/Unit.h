#pragma once
#include "MovingEntity.h"
#include "IJ.h"
#include "ICollideable.h"

class GameWorld;
class WorldRenderer;
struct CollisionData;

class Unit : public MovingEntity, public ICollideable {
	bool _is_selected{ false };
	int _id{ -1 };
public:
	Unit(IJ at_cell, WorldRenderer& world_renderer, GameWorld& game_world, int id);

	virtual ~Unit();

	void select();

	void deselect();

	void draw(WorldRenderer& world_renderer) override;

	void shoot_at(IJ target_cell, WorldRenderer& world_renderer);

	int id() const;

	void on_collision(const CollisionData& collision_data);
};