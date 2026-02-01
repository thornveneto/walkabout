#pragma once
#include "Vector2D.h"
#include "WorldRenderer.h"
#include "Effect.h"
class Explosion : public Effect {
    bool _alive{ true };
    float _r{ 0.f };
public:
    Vector2D centroid;


    Explosion(Vector2D centroid) : centroid{ centroid } {}

    void update(sf::Time& deltaTime) {
        if (!_alive) return;

        _r += 0.1f;

        if (_r > 30.f) {
            _alive = false;
        }
    }

    bool is_alive() {
        return _alive;
    }

    void draw(WorldRenderer& world_renderer) {
        Vector2D screen_point = world_renderer.tm * centroid;

        //std::cout << centroid.x << "," << centroid.y << std::endl;
        if (_alive) {
            world_renderer.draw_circle(
                screen_point.x,
                screen_point.y,
                _r,
                sf::Color::Magenta, sf::Color::Black
            );
        }

    }
};