#include "Weapon.h"
#include "../GameWorld.h"
#include "../WorldRenderer.h"
#include "../math/Vector2D.h"
#include "Unit.h"


Weapon::Weapon(GameWorld& game_world, IdType owner_id, bool is_melee) : _game_world{ game_world }, _owner_id{ owner_id }, _is_melee{ is_melee } {}

void Weapon::attack(IJ source_cell, IJ target_cell, WorldRenderer& world_renderer)
{
	if (is_melee()) {

		if (!(abs(source_cell.i - target_cell.i) <= 1 && abs(source_cell.j - target_cell.j) <= 1)) {
			std::cerr << "Attempt to use melee weapon over distance" << std::endl;
			return;
		}

		_update_frame = 1;

		Unit* attacked_unit = _game_world.terrain.unit_at(target_cell);

		if (attacked_unit) {
			int damage_value = 75;
			attacked_unit->apply_damage(damage_value);
		}
	}
	else {
		_game_world.spawn_projectile(source_cell, target_cell, _owner_id);
	}
}

bool Weapon::is_melee() const {
	return _is_melee;
}

void Weapon::update() {
	if (_update_frame != 0) {

		++_update_frame;

		if (_update_frame >= 30) {
			_update_frame = 0;
		}
	}
}

void Weapon::draw(WorldRenderer& world_renderer, const Vector2D& screen_point) {
	if (_is_melee) {
		world_renderer.draw_circle(
			screen_point.x,
			screen_point.y - world_renderer.cell_height / 2,
			(world_renderer.hh / 2) + (_update_frame  / 30.f) * world_renderer.hh,
			sf::Color::Red,
			sf::Color::Black
		);
	}
	else {
		world_renderer.draw_ellipse(
			screen_point.x,
			screen_point.y - world_renderer.cell_height / 2,
			world_renderer.hh,
			world_renderer.hh / 2,
			sf::Color::Red,
			sf::Color::Black
		);
	}
}

bool Weapon::more_updates() const {
	return _update_frame != 0;
}