#include "GameWorld.h"
#include "Explosion.h"
#include "Warrior.h"
#include "Projectile.h"

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

    spawn_warrior({ 0, 0 }, world_renderer);
    spawn_warrior({ 2, 0 }, world_renderer);
    //spawn_projectile(4, 0, 4, 9, world_renderer);
}

int GameWorld::spawn_warrior(IJ at_tile, WorldRenderer& world_renderer) {

    int warrior_id = allocate_entity_id();


    entity_map.emplace(warrior_id, std::make_unique<Warrior>(at_tile, world_renderer, *this));

    terrain.tile_at(at_tile).add_guest(entity_map[warrior_id].get());

    return warrior_id;
}

int GameWorld::spawn_projectile(IJ at_tile, IJ target_tile, WorldRenderer& world_renderer) {
    int projectile_id = allocate_entity_id();

    entity_map.emplace(projectile_id, std::make_unique<Projectile>(at_tile, world_renderer, *this));
    entity_map.at(projectile_id)->set_target(target_tile, world_renderer);

    //TODO: add guest??

    return projectile_id;
}

void GameWorld::spawn_explosion(Vector2D centroid) {
    effects.push_back(
        std::make_unique<Explosion>(centroid)
    );
}

void GameWorld::update_entities(sf::Time& delta_time, WorldRenderer& world_renderer) {
    for (auto& entity : entity_map) {
        entity.second->update(delta_time, world_renderer);

        IJ previous_home = entity.second->get_home_ij();
        IJ new_home = world_renderer.tile_ij_from_centroid(entity.second->centroid);

        entity.second->set_home(new_home);

        terrain.transfer_guest(previous_home, new_home, entity.second.get());
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

    for (const auto& entity : entity_map) {
        entity.second->draw(world_renderer);
    }

    for (auto& effect : effects) {
        effect->draw(world_renderer);
    }
}

void GameWorld::check_collisions(WorldRenderer& world_renderer) {
    //Tile& todo_smashed_tile = terrain.tile_at(4, 4);
    //LineSegment& west_wall = todo_smashed_tile.west_slot_segment(world_renderer);

    for (auto& entity : entity_map) {

        std::vector<IJ> terrain_collisions = terrain.terrain_collisions(entity.second->move_delta_segment(), world_renderer);

        for (const auto& collided_element : terrain_collisions) {
            //TODO: shouldn't actually be at, but at intersection point
            spawn_explosion(entity.second->centroid);
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