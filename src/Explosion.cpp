#include "Explosion.h"
#include "WorldRenderer.h"

Explosion::Explosion(Vector2D centroid) : _centroid{ centroid } {}

Explosion::~Explosion() = default;

void Explosion::draw(WorldRenderer& world_renderer) {
    Vector2D screen_point = world_renderer.calculate_screen_point(_centroid);

    //std::cout << centroid.x << "," << centroid.y << std::endl;
    if (_alive) {
        world_renderer.draw_circle(
            screen_point.x,
            screen_point.y - world_renderer.cell_height/2,
            _r,
            sf::Color::Magenta, sf::Color::Black
        );
    }

}

bool Explosion::is_alive() const {
    return _alive;
}

void Explosion::update(sf::Time& deltaTime) {
    if (!_alive) return;

    _r += 0.1f;

    constexpr float EXPLOSION_SIZE{ 15.f };

    if (_r > EXPLOSION_SIZE) {
        _alive = false;
    }
}