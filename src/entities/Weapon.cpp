#include "Weapon.h"
#include "../GameWorld.h"
#include "../WorldRenderer.h"


Weapon::Weapon(GameWorld& game_world, IdType owner_id, bool is_melee) : _game_world{ game_world }, _owner_id{ owner_id }, _is_melee{ is_melee } {}

void Weapon::attack(IJ source_cell, IJ target_cell, WorldRenderer& world_renderer)
{
	if (is_melee() && !(abs(source_cell.i - target_cell.i) <= 1 && abs(source_cell.j - target_cell.j) <= 1)) {
		std::cerr << "Attempt to use melee weapon over distance" << std::endl;

		return;
	}
	_game_world.spawn_projectile(source_cell, target_cell, world_renderer, _owner_id);
}

bool Weapon::is_melee() const {
	return _is_melee;
}