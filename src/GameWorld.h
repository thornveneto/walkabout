#pragma once
#include <SFML/System.hpp>   // REQUIRED

#include "Terrain.h"
#include <map>
#include <memory>
#include "IJ.h"
#include "Vector2D.h"

class WorldRenderer;
class Unit;
class Projectile;
class Effect;

struct GameWorldInternal;

class GameWorld {
    int _new_entity_id{ 1 };

    sf::Clock clock; // starts the clock
    bool paused = true;

    sf::Time get_delta_time();//WARNING destructive. Call once only
    std::vector<std::unique_ptr<Effect>> effects;
    std::map<int, std::unique_ptr<Unit>> units_map;
    //std::map<int, std::unique_ptr<Projectile>> projectiles_map;
    std::unique_ptr<GameWorldInternal> _storage;

    void check_out_of_bounds(WorldRenderer& world_renderer);
public:
    GameWorld();
    ~GameWorld();

    void update(WorldRenderer& world_renderer);

    void pause();
    void unpause();
    bool is_paused() const;


    Terrain terrain;

    int allocate_entity_id();

    void init(WorldRenderer& world_renderer);

    void spawn_explosion(Vector2D centroid);

    int spawn_unit(IJ at_cell, WorldRenderer& world_renderer);

    int spawn_projectile(IJ at_cell, IJ target_cell, WorldRenderer& world_renderer, int owner_id);

    void update_entities(sf::Time& delta_time, WorldRenderer& world_renderer);

    void update_effects(sf::Time& delta_time);

    void check_collisions(WorldRenderer& world_renderer);

    void draw(WorldRenderer& world_renderer);

    void sweep_pending_elements();

    bool any_more_updates() const;
};