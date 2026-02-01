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

    void update(sf::Time& deltaTime) override;

    bool is_alive() const override;

    void draw(WorldRenderer& world_renderer) override;
};