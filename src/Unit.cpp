#include "Unit.h"
#include "GameWorld.h"
#include "WorldRenderer.h"
#include "CollisionData.h"

Unit::~Unit() = default;

Unit::Unit(IJ at_cell, WorldRenderer& world_renderer, GameWorld& game_world, int id) : MovingEntity(at_cell, world_renderer, game_world), _id{ id } {

}

int Unit::id() const {
    return _id;
}

void Unit::select() {
    _is_selected = true;
}

void Unit::deselect() {
    _is_selected = false;
}

void Unit::draw(WorldRenderer& world_renderer) {
    Vector2D screen_point = world_renderer.tm * centroid;

    world_renderer.draw_circle(
        screen_point.x,
        screen_point.y - world_renderer.cell_height/2,
        world_renderer.hh,
        _is_selected ? sf::Color::Yellow : sf::Color::Blue, 
        sf::Color::Black
    );
}

void Unit::shoot_at(IJ target_cell, WorldRenderer& world_renderer) {
    _game_world.spawn_projectile(cell_ij, target_cell, world_renderer, id());
}

void Unit::on_collision(const CollisionData& collision_data) {
    
}