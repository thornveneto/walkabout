#include "Projectile.h"
#include "WorldRenderer.h"
#include "GameWorld.h"
#include "Vector2D.h"

Projectile::~Projectile() = default;

Projectile::Projectile(IJ at_cell, WorldRenderer& world_renderer, GameWorld& game_world, IdType owner_id) :
    MovingEntity(at_cell, world_renderer, game_world), _owner_id{owner_id}
{

}

IdType Projectile::owner_id() const {
    return _owner_id;
}

void Projectile::draw(WorldRenderer& world_renderer) {
    Vector2D screen_point = world_renderer.tm * centroid;

    //std::cout << "Projectile centroid: " << centroid.x << "," << centroid.y << std::endl;

    world_renderer.draw_circle(
        screen_point.x,
        screen_point.y - world_renderer.cell_height/2,
        _projectile_radius,
        sf::Color::Black, sf::Color::Red
    );
}

void Projectile::mark_for_sweep() {
    _is_pending_sweep = true;
}

bool Projectile::is_pending_sweep() const {
    return _is_pending_sweep;
}

void Projectile::on_collision(const CollisionData& collision_data) {
    mark_for_sweep();
}