#include "GameWorld.h"
#include "Explosion.h"
#include "WorldRenderer.h"
#include "Unit.h"
#include "Projectile.h"
#include "Effect.h"
#include <deque>
#include "CollisionData.h"

struct GameWorldInternal {
    std::map<IdType, std::unique_ptr<Projectile>> projectiles_map;
};

GameWorld::GameWorld() : _storage(std::make_unique<GameWorldInternal>()) {}

GameWorld::~GameWorld() = default;

void GameWorld::pause() {
    paused = true;
}

void GameWorld::unpause() {
    paused = false;
    clock.restart();
}

bool GameWorld::is_paused() const {
    return paused;
}

sf::Time GameWorld::get_delta_time() {
    sf::Time delta_time = clock.restart();

    return delta_time;
}

int GameWorld::allocate_entity_id() {
    return _new_entity_id++;
}

void GameWorld::init(WorldRenderer& world_renderer) {
    terrain.init();

    spawn_unit({ 0, 0 }, world_renderer);
    spawn_unit({ 2, 0 }, world_renderer);
    //spawn_projectile(4, 0, 4, 9, world_renderer);
}

IdType GameWorld::spawn_unit(IJ at_cell, WorldRenderer& world_renderer) {

    IdType unit_id = allocate_entity_id();


    units_map.emplace(unit_id, std::make_unique<Unit>(at_cell, world_renderer, *this, unit_id));

    terrain.cell_at(at_cell).add_guest(units_map[unit_id].get());

    return unit_id;
}

IdType GameWorld::spawn_projectile(IJ at_cell, IJ target_cell, WorldRenderer& world_renderer, int owner_id) {
    IdType projectile_id = allocate_entity_id();

    _storage->projectiles_map.emplace(projectile_id, std::make_unique<Projectile>(at_cell, world_renderer, *this, owner_id));
    _storage->projectiles_map.at(projectile_id)->set_target(target_cell, world_renderer);

    //TODO: add guest??

    return projectile_id;
}

void GameWorld::spawn_explosion(Vector2D centroid) {
    effects.push_back(
        std::make_unique<Explosion>(centroid)
    );
}

void GameWorld::update_entities(sf::Time& delta_time, WorldRenderer& world_renderer) {
    for (auto& unit : units_map) {
        unit.second->update(delta_time, world_renderer);

        IJ previous_home = unit.second->get_home_ij();
        IJ new_home = world_renderer.tile_ij_from_centroid(unit.second->centroid);

        unit.second->set_home(new_home);

        unit.second->execute_waypoint_logic(world_renderer);//TODO: CRASH BOOM BANG

        terrain.transfer_guest_unit(previous_home, new_home, unit.second.get());
    }

    //CHECK
    for (auto& projectile : _storage->projectiles_map) {
        projectile.second->update(delta_time, world_renderer);

        IJ previous_home = projectile.second->get_home_ij();
        IJ new_home = world_renderer.tile_ij_from_centroid(projectile.second->centroid);

        projectile.second->set_home(new_home);

        //no guests for projectiles - terrain.transfer_guest(previous_home, new_home, projectile.second.get());
    }
}

void GameWorld::update_effects(sf::Time& delta_time) {

    //Updating effects
    for (auto& effect : effects) {
        effect->update(delta_time);
    }

    effects.erase(
        std::remove_if(
            effects.begin(), effects.end(),
            [](const auto& e) { return !e->is_alive(); }
        ),
        effects.end()
    );
}

void GameWorld::draw(WorldRenderer& world_renderer) {
    //Draw here
    terrain.draw(world_renderer);

    for (const auto& unit : units_map) {
        unit.second->draw(world_renderer);
    }

    //CHECK
    for (const auto& projectile : _storage->projectiles_map) {
        projectile.second->draw(world_renderer);
    }

    for (auto& effect : effects) {
        effect->draw(world_renderer);
    }
}

void GameWorld::check_collisions(WorldRenderer& world_renderer) {
    //Checking projectile collisions
    for (auto& projectile : _storage->projectiles_map) {

        //First terrain collisions
        std::vector<IJ> terrain_collisions = terrain.terrain_collisions(projectile.second->move_delta_segment(), world_renderer);

        for (const auto& collided_element : terrain_collisions) {
            //TODO: shouldn't actually be at, but at intersection point
            spawn_explosion(projectile.second->centroid);

            projectile.second->on_collision({-1});
        }

        //Second - projectiles hitting units
        Unit* unit_hit = terrain.unit_collision(projectile.second->move_delta_segment(), world_renderer);

        if (unit_hit && projectile.second->owner_id() != unit_hit->id()) {
            spawn_explosion(projectile.second->centroid);

            projectile.second->on_collision({ -1 });
            unit_hit->on_collision({ -1 });
        }
    }
}

void GameWorld::check_out_of_bounds(WorldRenderer& world_renderer) {
    for (const auto& projectile_item : _storage->projectiles_map) {
        
        if (!terrain.within_boundaries(projectile_item.second->centroid, world_renderer)) {
            projectile_item.second->mark_for_sweep();
        }
    }
}

void GameWorld::update(WorldRenderer& world_renderer) {
    if (!is_paused()) {
        sf::Time delta_time = get_delta_time();

        update_entities(delta_time, world_renderer);

        check_collisions(world_renderer);

        check_out_of_bounds(world_renderer);

        update_effects(delta_time);
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
    if (!effects.empty()) {
        return true;
    }

    for (auto& unit : units_map) {
        if (!unit.second->is_stopped()) {
            return true;
        }
    }

    //BR - projectiles must disappear
    if (!_storage->projectiles_map.empty()) {
        return true;
    }

    return false;
}