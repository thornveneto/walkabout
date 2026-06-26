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

GameWorld::GameWorld(WorldRenderer& world_renderer) : m_storage(std::make_unique<GameWorldInternal>()), m_world_renderer{ world_renderer }, terrain(world_renderer) {}

GameWorld::~GameWorld() = default;

void GameWorld::pause() {
    m_paused = true;
}

void GameWorld::unpause() {
    m_paused = false;
    m_clock.restart();
}

bool GameWorld::is_paused() const {
    return m_paused;
}

sf::Time GameWorld::get_delta_time() {
    sf::Time delta_time = m_clock.restart();

    return delta_time;
}

int GameWorld::allocate_entity_id() {
    return m_new_entity_id++;
}

Team* GameWorld::get_team(IdType team_id) {
    return m_teams.at(team_id).get();
}

void GameWorld::init() {
    terrain.init();

    //TODO: messy id management
    m_teams.emplace(1, std::make_unique<Team>(1, sf::Color::Blue));
    m_teams.emplace(2, std::make_unique<Team>(2, sf::Color::White));

    m_teams.at(1).get()->add_player(spawn_unit({0, 0}, m_teams.at(1).get()));

    m_teams.at(2).get()->add_player(spawn_unit({ 2, 0 }, m_teams.at(2).get()));
    //spawn_projectile(4, 0, 4, 9, world_renderer);
}

IdType GameWorld::spawn_unit(IJ at_cell, Team* team) {

    IdType unit_id = allocate_entity_id();


    m_units_map.emplace(unit_id, std::make_unique<Unit>(at_cell, m_world_renderer, *this, unit_id, team->color()));

    m_units_map.at(unit_id)->equip_main_weapon(false, 1); //TODO: find a better place
    m_units_map.at(unit_id)->equip_aux_weapon(true, 2); //TODO: find a better place
    //_units_map.at(unit_id)->activate_main_weapon();

    terrain.cell_at(at_cell).add_guest(m_units_map[unit_id].get());

    return unit_id;
}

void GameWorld::reset_team_action_points(IdType team_id) {
    for (const auto& [id, unit] : m_units_map) {
        if (m_teams[team_id]->has_player(id)) {
            unit->reset_action_points();
        }
    }
}

IdType GameWorld::spawn_projectile(IJ at_cell, IJ target_cell, int owner_id) {
    IdType projectile_id = allocate_entity_id();

    m_storage->projectiles_map.emplace(projectile_id, std::make_unique<Projectile>(at_cell, m_world_renderer, *this, owner_id));
    m_storage->projectiles_map.at(projectile_id)->set_target(target_cell, m_world_renderer);

    //TODO: add guest??

    return projectile_id;
}

void GameWorld::spawn_explosion(Vector2D centroid) {
    m_effects.push_back(
        std::make_unique<Explosion>(centroid)
    );
}

void GameWorld::update_entities(sf::Time& delta_time) {
    for (auto& unit : m_units_map) {
        unit.second->update(delta_time, m_world_renderer);

        IJ previous_home = unit.second->get_home_ij();
        IJ new_home = m_world_renderer.tile_ij_from_centroid(unit.second->centroid());

        unit.second->set_home(new_home);

        unit.second->execute_waypoint_logic(m_world_renderer);//TODO: CRASH BOOM BANG

        terrain.transfer_guest_unit(previous_home, new_home, unit.second.get());
    }

    //CHECK
    for (auto& projectile : m_storage->projectiles_map) {
        projectile.second->update(delta_time, m_world_renderer);

        IJ previous_home = projectile.second->get_home_ij();
        IJ new_home = m_world_renderer.tile_ij_from_centroid(projectile.second->centroid());

        projectile.second->set_home(new_home);

        //no guests for projectiles - terrain.transfer_guest(previous_home, new_home, projectile.second.get());
    }
}

void GameWorld::update_effects(sf::Time& delta_time) {

    //Updating effects
    for (auto& effect : m_effects) {
        effect->update(delta_time);
    }

    m_effects.erase(
        std::remove_if(
            m_effects.begin(), m_effects.end(),
            [](const auto& e) { return !e->is_alive(); }
        ),
        m_effects.end()
    );
}

const std::map<IdType, std::unique_ptr<Unit>>& GameWorld::units_map() const {
    return m_units_map;
}

const std::vector<std::unique_ptr<Effect>>& GameWorld::effects() const {
    return m_effects;
}

const std::map<int, std::unique_ptr<Projectile>>& GameWorld::projectiles_map() const {
    //TODO: check if we still need these bonfire dances with _storage
    return m_storage->projectiles_map;
}

void GameWorld::check_collisions() {
    //Checking projectile collisions
    for (auto& projectile : m_storage->projectiles_map) {

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
    for (const auto& projectile_item : m_storage->projectiles_map) {
        
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

    for (const auto& projectile_item : m_storage->projectiles_map) {

        if (projectile_item.second->is_pending_sweep()) {
            sweep_projectile_id_queue.push_back(projectile_item.first);
        }
    }

    for (int projectile_id : sweep_projectile_id_queue) {
        m_storage->projectiles_map.erase(projectile_id);
    }
}

bool GameWorld::any_more_updates() const {
    if (!m_effects.empty()) {
        return true;
    }

    for (auto& unit : m_units_map) {
        if (unit.second->any_more_updates()) {
            return true;
        }
    }

    //BR - projectiles must disappear
    if (!m_storage->projectiles_map.empty()) {
        return true;
    }

    return false;
}