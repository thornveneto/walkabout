#include "Projectile.h"
#include "WorldRenderer.h"
#include "GameWorld.h"
#include "Vector2D.h"

Projectile::~Projectile() = default;

Projectile::Projectile(IJ at_cell, WorldRenderer& world_renderer, GameWorld& game_world) : MovingEntity(at_cell, world_renderer, game_world) {

}

void Projectile::draw(WorldRenderer& world_renderer) {
    Vector2D screen_point = world_renderer.tm * centroid;

    const float projectile_radius = 5.f;

    //std::cout << "Projectile centroid: " << centroid.x << "," << centroid.y << std::endl;

    world_renderer.draw_circle(
        screen_point.x,
        screen_point.y - world_renderer.cell_height/2,
        projectile_radius,
        sf::Color::Black, sf::Color::Black
    );
}