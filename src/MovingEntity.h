#pragma once
#include <vector>
#include <utility>
#include <SFML/Graphics.hpp>

#include "Vector2D.h"
#include "WorldRenderer.h"
#include "Matrix2D.h"
#include "LineSegment.h"
#include "GameWorld.h"
#include "IJ.h"
class MovingEntity {
public:
    GameWorld& _game_world;
    Vector2D prev_centroid;
    Vector2D centroid;
    Vector2D speed_vector{ 0, 0 };

    IJ tile_ij;

    int finish_tile_i{ 15 };
    int finish_tile_j{ 15 };
    int max_speed{ 20 };

    int waypoint_id{ -1 };
    std::vector<IJ> waypoints;


    MovingEntity(IJ home_tile, WorldRenderer& world_renderer, GameWorld& game_world) :
        _game_world{ game_world }, tile_ij{ home_tile }, centroid{ world_renderer.tile_centroid_from_ij(home_tile) } {
        //TODO: a bit ugly and only because we need to know the tile size
    }
    //
    void set_waypoints(const std::vector<IJ>& new_path);

    void start_waypoints_following(WorldRenderer& world_renderer);

    void execute_waypoint_logic(WorldRenderer& world_renderer);

    Vector2D home_centroid(WorldRenderer& world_renderer);

    Vector2D target_centroid(WorldRenderer& world_renderer);

    LineSegment home_target_segment(WorldRenderer& world_renderer);

    LineSegment move_delta_segment();

    void set_home(IJ home);
    void set_home_and_center(IJ home, WorldRenderer& world_renderer);
    IJ get_home_ij() const;

    void set_target(IJ target, WorldRenderer& world_renderer);

    bool reached_target(WorldRenderer& world_renderer);

    void update(sf::Time& deltaTime, WorldRenderer& world_renderer);

    virtual void draw(WorldRenderer& world_renderer);
};