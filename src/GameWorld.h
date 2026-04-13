#pragma once
#include <SFML/System.hpp>   // REQUIRED

#include "terrain/Terrain.h"
#include <map>
#include <memory>
#include "math/IJ.h"
#include "math/Vector2D.h"
#include "_types.h"

class WorldRenderer;
class Unit;
class Projectile;
class Effect;

struct GameWorldInternal;

class GameWorld {
public:
    Terrain terrain;

    GameWorld(WorldRenderer& world_renderer);
    ~GameWorld();

    void init();

    void update();

    bool any_more_updates() const;

    /*
        Game clock management
    */
    void pause();
    void unpause();
    bool is_paused() const; //TODO: move to GameStateDesc

    /*
        Entity factory methods
    */
    void spawn_explosion(Vector2D centroid);

    IdType spawn_unit(IJ at_cell);

    IdType spawn_projectile(IJ at_cell, IJ target_cell, int owner_id);

    void sweep_pending_elements();

    /*
        Physics
    */
    void check_collisions();

    const std::vector<std::unique_ptr<Effect>>& effects() const;
    const std::map<IdType, std::unique_ptr<Unit>>& units_map() const;
    const std::map<int, std::unique_ptr<Projectile>>& projectiles_map() const;
private:
    IdType _new_entity_id{ 1 };

    sf::Clock _clock; // starts the clock
    bool _paused = true;

    sf::Time get_delta_time();//WARNING destructive. Call once only
    std::vector<std::unique_ptr<Effect>> _effects;
    std::map<IdType, std::unique_ptr<Unit>> _units_map;
    //std::map<int, std::unique_ptr<Projectile>> projectiles_map;
    std::unique_ptr<GameWorldInternal> _storage;

    WorldRenderer& world_renderer;

    void check_out_of_bounds();

    void update_entities(sf::Time& delta_time);

    void update_effects(sf::Time& delta_time);

    int allocate_entity_id();
};