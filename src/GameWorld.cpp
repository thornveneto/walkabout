#include "GameWorld.h"
#include "effects/Explosion.h"
#include "WorldRenderer.h"
#include "entities/Unit.h"
#include "entities/Projectile.h"
#include "effects/Effect.h"
#include <deque>
#include "physics/CollisionData.h"

struct GameWorldInternal {
    std::map<IdType, std::unique_ptr<Projectile>> projectiles_map;
};

GameWorld::GameWorld(WorldRenderer& world_renderer) : _storage(std::make_unique<GameWorldInternal>()), world_renderer{ world_renderer }, terrain(world_renderer) {}

GameWorld::~GameWorld() = default;

void GameWorld::pause() {
    paused = true;
}

void GameWorld::unpause() {
    paused = false;
    _clock.restart();
}

bool GameWorld::is_paused() const {
    return paused;
}

sf::Time GameWorld::get_delta_time() {
    sf::Time delta_time = _clock.restart();

    return delta_time;
}

int GameWorld::allocate_entity_id() {
    return _new_entity_id++;
}

void GameWorld::init() {
    terrain.init();

    spawn_unit({ 0, 0 });
    spawn_unit({ 2, 0 });
    //spawn_projectile(4, 0, 4, 9, world_renderer);
}

IdType GameWorld::spawn_unit(IJ at_cell) {

    IdType unit_id = allocate_entity_id();


    _units_map.emplace(unit_id, std::make_unique<Unit>(at_cell, world_renderer, *this, unit_id));

    _units_map.at(unit_id)->equip_main_weapon(false); //TODO: find a better place
    _units_map.at(unit_id)->equip_aux_weapon(true); //TODO: find a better place
    //_units_map.at(unit_id)->activate_main_weapon();

    terrain.cell_at(at_cell).add_guest(_units_map[unit_id].get());

    return unit_id;
}

IdType GameWorld::spawn_projectile(IJ at_cell, IJ target_cell, int owner_id) {
    IdType projectile_id = allocate_entity_id();

    _storage->projectiles_map.emplace(projectile_id, std::make_unique<Projectile>(at_cell, world_renderer, *this, owner_id));
    _storage->projectiles_map.at(projectile_id)->set_target(target_cell, world_renderer);

    //TODO: add guest??

    return projectile_id;
}

void GameWorld::spawn_explosion(Vector2D centroid) {
    _effects.push_back(
        std::make_unique<Explosion>(centroid)
    );
}

void GameWorld::update_entities(sf::Time& delta_time) {
    for (auto& unit : _units_map) {
        unit.second->update(delta_time, world_renderer);

        IJ previous_home = unit.second->get_home_ij();
        IJ new_home = world_renderer.tile_ij_from_centroid(unit.second->centroid());

        unit.second->set_home(new_home);

        unit.second->execute_waypoint_logic(world_renderer);//TODO: CRASH BOOM BANG

        terrain.transfer_guest_unit(previous_home, new_home, unit.second.get());
    }

    //CHECK
    for (auto& projectile : _storage->projectiles_map) {
        projectile.second->update(delta_time, world_renderer);

        IJ previous_home = projectile.second->get_home_ij();
        IJ new_home = world_renderer.tile_ij_from_centroid(projectile.second->centroid());

        projectile.second->set_home(new_home);

        //no guests for projectiles - terrain.transfer_guest(previous_home, new_home, projectile.second.get());
    }
}

void GameWorld::update_effects(sf::Time& delta_time) {

    //Updating effects
    for (auto& effect : _effects) {
        effect->update(delta_time);
    }

    _effects.erase(
        std::remove_if(
            _effects.begin(), _effects.end(),
            [](const auto& e) { return !e->is_alive(); }
        ),
        _effects.end()
    );
}

const std::map<IdType, std::unique_ptr<Unit>>& GameWorld::units_map() const {
    return _units_map;
}

const std::vector<std::unique_ptr<Effect>>& GameWorld::effects() const {
    return _effects;
}

const std::map<int, std::unique_ptr<Projectile>>& GameWorld::projectiles_map() const {
    //TODO: check if we still need these bonfire dances with _storage
    return _storage->projectiles_map;
}

void GameWorld::check_collisions() {
    //Checking projectile collisions
    for (auto& projectile : _storage->projectiles_map) {

        //First terrain collisions
        std::vector<IJ> terrain_collisions = terrain.terrain_collisions(projectile.second->move_delta_segment());

        for (const auto& collided_element : terrain_collisions) {
            //TODO: shouldn't actually be at, but at intersection point
            spawn_explosion(projectile.second->centroid());

            projectile.second->on_collision({-1});
        }

        //Second - projectiles hitting units
        Unit* unit_hit = terrain.unit_collision(projectile.second->move_delta_segment());

        if (unit_hit && projectile.second->owner_id() != unit_hit->id()) {
            spawn_explosion(projectile.second->centroid());

            projectile.second->on_collision({ -1 });
            unit_hit->on_collision({ -1, projectile.second->damage_power()});
        }
    }
}

void GameWorld::check_out_of_bounds() {
    for (const auto& projectile_item : _storage->projectiles_map) {
        
        if (!terrain.within_boundaries(projectile_item.second->centroid())) {
            projectile_item.second->mark_for_sweep();
        }
    }
}

void GameWorld::update() {
    if (!is_paused()) {
        sf::Time delta_time = get_delta_time();

        update_entities(delta_time);

        check_collisions();

        check_out_of_bounds();

        update_effects(delta_time);

        if (!any_more_updates()) {
            pause();
        }
    }
}

void GameWorld::sweep_pending_elements() {
    //TODO: suboptimal loop and repeating the action
    std::deque<int> sweep_projectile_id_queue;

    for (const auto& projectile_item : _storage->projectiles_map) {

        if (projectile_item.second->is_pending_sweep()) {
            sweep_projectile_id_queue.push_back(projectile_item.first);
        }
    }

    for (int projectile_id : sweep_projectile_id_queue) {
        _storage->projectiles_map.erase(projectile_id);
    }
}

bool GameWorld::any_more_updates() const {
    if (!_effects.empty()) {
        return true;
    }

    for (auto& unit : _units_map) {
        if (unit.second->any_more_updates()) {
            return true;
        }
    }

    //BR - projectiles must disappear
    if (!_storage->projectiles_map.empty()) {
        return true;
    }

    return false;
}