#pragma once
#include "../math/IJ.h"
#include "../_types.h"
class GameWorld;
class WorldRenderer;

class Weapon {
public:
	Weapon(GameWorld& game_world, IdType owner_id, bool is_melee);

	void attack(IJ source_cell, IJ target_cell, WorldRenderer& world_renderer);
	bool is_melee() const; //TODO: bad design makes assumptions about an invariant
private:
	GameWorld& _game_world;
	IdType _owner_id;
	bool _is_melee;
};