#pragma once
#include <vector>
#include <utility>
#include <SFML/Graphics.hpp>

#include "../math/Vector2D.h"
#include "../math/Matrix2D.h"
#include "../math/LineSegment.h"
#include "../math/IJ.h"

class GameWorld;
class WorldRenderer;

class MovingEntity {
    Vector2D _prev_centroid;
    int _waypoint_id{ -1 };
    std::vector<IJ> _waypoints;
    IJ _cell_ij;
    Vector2D _centroid;
protected:
    GameWorld& _game_world;
public:

    Vector2D speed_vector{ 0, 0 };



    int finish_tile_i{ 15 };//TODO: merge into one
    int finish_tile_j{ 15 };
    int max_speed{ 20 };

    MovingEntity(IJ home_cell, WorldRenderer& world_renderer, GameWorld& game_world);

    virtual ~MovingEntity();

    //
    void set_waypoints(const std::vector<IJ>& new_path);

    void start_waypoints_following(WorldRenderer& world_renderer);

    void execute_waypoint_logic(WorldRenderer& world_renderer);

    Vector2D centroid() const;

    Vector2D home_centroid(WorldRenderer& world_renderer); //centroid of the home tile

    Vector2D target_centroid(WorldRenderer& world_renderer); //centroid of the target tile

    LineSegment home_target_segment(WorldRenderer& world_renderer);

    LineSegment move_delta_segment();

    void set_home(IJ home);
    void set_home_and_center(IJ home, WorldRenderer& world_renderer);
    IJ get_home_ij() const;

    void set_target(IJ target, WorldRenderer& world_renderer);

    bool reached_target(WorldRenderer& world_renderer);

    void update(sf::Time& deltaTime, WorldRenderer& world_renderer);

    virtual void draw(WorldRenderer& world_renderer);

    void stop();
    bool is_stopped() const;
};