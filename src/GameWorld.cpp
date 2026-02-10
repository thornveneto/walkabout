#include "GameWorld.h"
#include "Explosion.h"
#include "WorldRenderer.h"
#include "Unit.h"
#include "Projectile.h"
#include "Effect.h"

struct GameWorldInternal {
    std::map<int, std::unique_ptr<Projectile>> projectiles_map;
};

GameWorld::GameWorld() : _storage(std::make_unique<GameWorldInternal>()) {}

GameWorld::~GameWorld() = default;


void GameWorld::toggle_pause() {
    paused = !paused;
    if (paused) {
        pause_start = clock.getElapsedTime();
    } else {
        clock.restart();
    }
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

int GameWorld::spawn_unit(IJ at_cell, WorldRenderer& world_renderer) {

    int warrior_id = allocate_entity_id();


    units_map.emplace(warrior_id, std::make_unique<Unit>(at_cell, world_renderer, *this));

    terrain.cell_at(at_cell).add_guest(units_map[warrior_id].get());

    return warrior_id;
}

int GameWorld::spawn_projectile(IJ at_cell, IJ target_cell, WorldRenderer& world_renderer) {
    std::cout << "SPAWN PROJECTILE AT " << at_cell.i << "-" << at_cell.j << std::endl;

    int projectile_id = allocate_entity_id();

    _storage->projectiles_map.emplace(projectile_id, std::make_unique<Projectile>(at_cell, world_renderer, *this));
    _storage->projectiles_map.at(projectile_id)->set_target(target_cell, world_renderer);

    std::cout << "projectile centroid" << _storage->projectiles_map.at(projectile_id)->centroid.x << " " << _storage->projectiles_map.at(projectile_id)->centroid.y << std::endl;

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

        terrain.transfer_guest_unit(previous_home, new_home, unit.second.get());
    }

    //CHECK
    for (auto& projectile : _storage->projectiles_map) {
        std::cout << "DELTA  " << delta_time.asMilliseconds() << ": ";

        std::cout << "BEFORE  " << projectile.second->centroid.x << " " << projectile.second->centroid.y;


        projectile.second->update(delta_time, world_renderer);

        std::cout << "   AFTER   " << projectile.second->centroid.x << " " << projectile.second->centroid.y << std::endl;

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
    //Tile& todo_smashed_tile = terrain.tile_at(4, 4);
    //LineSegment& west_wall = todo_smashed_tile.west_slot_segment(world_renderer);

    //CHECK
    for (auto& projectile : _storage->projectiles_map) {

        std::vector<IJ> terrain_collisions = terrain.terrain_collisions(projectile.second->move_delta_segment(), world_renderer);

        for (const auto& collided_element : terrain_collisions) {
            //TODO: shouldn't actually be at, but at intersection point
            spawn_explosion(projectile.second->centroid);
        }
    }
}

void GameWorld::update(WorldRenderer& world_renderer) {
    if (!is_paused()) {
        sf::Time delta_time = get_delta_time();

        //Custom
        /*
        game_world.entity_map.at(1)->execute_waypoint_logic(world_renderer);
        */

        update_entities(delta_time, world_renderer);

        check_collisions(world_renderer);

        update_effects(delta_time);
    }
}