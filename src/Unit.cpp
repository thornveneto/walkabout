#include "Unit.h"

void Unit::draw(WorldRenderer& world_renderer) {
    Vector2D screen_point = world_renderer.tm * centroid;

    world_renderer.draw_circle(
        screen_point.x,
        screen_point.y,
        5.f,
        sf::Color::Blue, sf::Color::Black
    );
}

void Unit::shoot_at(IJ target_cell, WorldRenderer& world_renderer) {
    _game_world.spawn_projectile(cell_ij, target_cell, world_renderer);
}