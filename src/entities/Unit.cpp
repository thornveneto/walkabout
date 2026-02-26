#include "Unit.h"
#include "../GameWorld.h"
#include "../WorldRenderer.h"
#include "../physics/CollisionData.h"
#include <SFML/Graphics.hpp>
#include "Weapon.h"

Unit::~Unit() = default;

Unit::Unit(IJ at_cell, WorldRenderer& world_renderer, GameWorld& game_world, IdType id) : MovingEntity(at_cell, world_renderer, game_world), _id{ id }, _active_weapon{ nullptr } {

}

void Unit::equip_main_weapon(bool is_melee) {
    _main_weapon = std::make_unique<Weapon>(_game_world, _id, is_melee);
}

void Unit::equip_aux_weapon(bool is_melee) {
    _aux_weapon = std::make_unique<Weapon>(_game_world, _id, is_melee);
}

void Unit::activate_main_weapon() {
    _active_weapon = _main_weapon.get();
}
void Unit::activate_aux_weapon() {
    _active_weapon = _aux_weapon.get();
}

Weapon* Unit::active_weapon() {
    return _active_weapon;
}

IdType Unit::id() const {
    return _id;
}

void Unit::select() {
    _is_selected = true;
}

void Unit::deselect() {
    _is_selected = false;
}

void Unit::update(sf::Time& deltaTime, WorldRenderer& world_renderer) {
    MovingEntity::update(deltaTime, world_renderer);

    if (_active_weapon) {
        _active_weapon->update();
    }    
}

bool Unit::any_more_updates() {
    if (!is_stopped()) {
        return true;
    }

    if (_active_weapon && _active_weapon->more_updates()) {
        return true;
    }

    return false;
}

void Unit::draw(WorldRenderer& world_renderer) {
    Vector2D screen_point = world_renderer.calculate_screen_point(centroid());

    if (is_alive()) {
        world_renderer.draw_circle(
            screen_point.x,
            screen_point.y - world_renderer.cell_height / 2,
            world_renderer.hh,
            _is_selected ? sf::Color::Yellow : sf::Color::Blue,
            sf::Color::Black
        );

        Weapon* unit_active_weapon = active_weapon();

        if (unit_active_weapon) {
            unit_active_weapon->draw(world_renderer, screen_point);
        }
    }
    else {
        world_renderer.draw_ellipse(
            screen_point.x,
            screen_point.y,
            world_renderer.hh,
            world_renderer.hh/2,
            sf::Color::Red,
            sf::Color::Black
        );
    }

}

void Unit::attack_at(IJ target_cell, WorldRenderer& world_renderer) {
    if (_active_weapon) {
        _active_weapon->attack(get_home_ij(), target_cell, world_renderer);
    }
}

void Unit::on_collision(const CollisionData& collision_data) {

    apply_damage(collision_data.damage_power);
}

void Unit::apply_damage(int damage_level) {
    _health = std::max(0, _health - damage_level);
}

bool Unit::is_alive() const {
    return _health > 0;
}

int Unit::health() const {
    return _health;
}

int Unit::max_health() const {
    return _MAX_HEALTH;
}