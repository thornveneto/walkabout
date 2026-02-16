#pragma once
#include "Vector2D.h"
#include "WorldRenderer.h"
#include "Effect.h"

class WorldRender;

class Explosion : public Effect {
    bool _alive{ true };
    float _r{ 0.f };
    Vector2D _centroid;
public:
    Explosion(Vector2D centroid);

    virtual ~Explosion();

    void update(sf::Time& deltaTime) override;

    bool is_alive() const override;

    void draw(WorldRenderer& world_renderer) override;
};