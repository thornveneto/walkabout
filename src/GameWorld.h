#pragma once
#include <SFML/System.hpp>   // REQUIRED

#include "Terrain.h"
#include <map>
#include <memory>
#include "Effect.h"
#include "IJ.h"

//#include "MovingEntity.h"
class MovingEntity;

class GameWorld {
    int _new_entity_id{ 1 };

    sf::Clock clock; // starts the clock
    bool paused = true;
    sf::Time pause_start;//TODO: we don't seem to use it anymore anywhere

    sf::Time get_delta_time();//WARNING destructive. Call once only

public:

    void update(WorldRenderer& world_renderer);

    void toggle_pause();
    bool is_paused() const;


    Terrain terrain;
    std::map<int, std::unique_ptr<MovingEntity>> entity_map;

    std::vector<std::unique_ptr<Effect>> effects;

    int allocate_entity_id();

    void init(WorldRenderer& world_renderer);

    void spawn_explosion(Vector2D centroid);

    int spawn_warrior(IJ at_cell, WorldRenderer& world_renderer);

    int spawn_projectile(IJ at_cell, IJ target_cell, WorldRenderer& world_renderer);

    void update_entities(sf::Time& delta_time, WorldRenderer& world_renderer);

    void update_effects(sf::Time& delta_time);

    void check_collisions(WorldRenderer& world_renderer);

    void draw(WorldRenderer& world_renderer);
};