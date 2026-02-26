#pragma once
#include "../math/IJ.h"
#include "../_types.h"
class GameWorld;
class WorldRenderer;
class Vector2D;

class Weapon {
public:
	Weapon(GameWorld& game_world, IdType owner_id, bool is_melee);

	void attack(IJ source_cell, IJ target_cell, WorldRenderer& world_renderer);
	bool is_melee() const; //TODO: bad design makes assumptions about an invariant

	void draw(WorldRenderer& world_renderer, const Vector2D& screen_point);

	bool more_updates() const;

	void update();
private:
	GameWorld& _game_world;
	IdType _owner_id;
	bool _is_melee;

	int _update_frame{ 0 }; //
};