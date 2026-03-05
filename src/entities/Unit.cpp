#include "Unit.h"
#include "../GameWorld.h"
#include "../WorldRenderer.h"
#include "../physics/CollisionData.h"
#include <SFML/Graphics.hpp>
#include "Weapon.h"

Unit::~Unit() = default;

Unit::Unit(IJ at_cell, WorldRenderer& world_renderer, GameWorld& game_world, IdType id) : MovingEntity(at_cell, world_renderer, game_world), m_id{ id }, m_active_weapon{ nullptr } {

}

void Unit::equip_main_weapon(bool is_melee) {
    m_main_weapon = std::make_unique<Weapon>(_game_world, m_id, is_melee);
}

void Unit::equip_aux_weapon(bool is_melee) {
    m_aux_weapon = std::make_unique<Weapon>(_game_world, m_id, is_melee);
}

void Unit::activate_main_weapon() {
    m_active_weapon = m_main_weapon.get();
}
void Unit::activate_aux_weapon() {
    m_active_weapon = m_aux_weapon.get();
}

Weapon* Unit::active_weapon() {
    return m_active_weapon;
}

IdType Unit::id() const {
    return m_id;
}

void Unit::select() {
    m_is_selected = true;
}

void Unit::deselect() {
    m_is_selected = false;
}

void Unit::update(sf::Time& deltaTime, WorldRenderer& world_renderer) {
    MovingEntity::update(deltaTime, world_renderer);

    if (m_active_weapon) {
        m_active_weapon->update();
    }    
}

bool Unit::any_more_updates() {
    if (!is_stopped()) {
        return true;
    }

    if (m_active_weapon && m_active_weapon->more_updates()) {
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
            m_is_selected ? sf::Color::Yellow : sf::Color::Blue,
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
    if (m_active_weapon) {
        m_active_weapon->attack(get_home_ij(), target_cell, world_renderer);
    }
}

void Unit::on_collision(const CollisionData& collision_data) {

    apply_damage(collision_data.damage_power);
}

void Unit::apply_damage(int damage_level) {
    m_health = std::max(0, m_health - damage_level);
}

bool Unit::is_alive() const {
    return m_health > 0;
}

int Unit::health() const {
    return m_health;
}

int Unit::max_health() const {
    return MAX_HEALTH;
}