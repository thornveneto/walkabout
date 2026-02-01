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

void Unit::shoot_at(IJ target_tile, WorldRenderer& world_renderer) {
    _game_world.spawn_projectile(tile_ij, target_tile, world_renderer);
}