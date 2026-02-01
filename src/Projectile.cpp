#include "Projectile.h"

void Projectile::draw(WorldRenderer& world_renderer) {
    Vector2D screen_point = world_renderer.tm * centroid;

    world_renderer.draw_circle(
        screen_point.x,
        screen_point.y,
        5.f,
        sf::Color::Black, sf::Color::Black
    );
}