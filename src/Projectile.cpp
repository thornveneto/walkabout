#include "Projectile.h"

void Projectile::draw(WorldRenderer& world_renderer) {
    Vector2D screen_point = world_renderer.tm * centroid;

    const float projectile_radius = 5.f;

    world_renderer.draw_circle(
        screen_point.x,
        screen_point.y - world_renderer.cell_height/2,
        projectile_radius,
        sf::Color::Black, sf::Color::Black
    );
}