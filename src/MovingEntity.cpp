#include "MovingEntity.h"
void MovingEntity::set_waypoints(const std::vector<IJ>& new_path) {
    waypoints = new_path;
}
void MovingEntity::start_waypoints_following(WorldRenderer& world_renderer) {
    waypoint_id = 0;
    set_home_and_center(waypoints[waypoint_id % waypoints.size()], world_renderer);

    ++waypoint_id;
    set_target(waypoints[waypoint_id % waypoints.size()], world_renderer);
}
void MovingEntity::execute_waypoint_logic(WorldRenderer& world_renderer) {
    //Updating stuff
    if (reached_target(world_renderer)) {
        set_home_and_center(waypoints[waypoint_id % waypoints.size()], world_renderer);

        ++waypoint_id;

        set_target(waypoints[waypoint_id % waypoints.size()], world_renderer);
    }
}

Vector2D MovingEntity::home_centroid(WorldRenderer& world_renderer) {
    return world_renderer.tile_centroid_from_ij(tile_ij);
}
Vector2D MovingEntity::target_centroid(WorldRenderer& world_renderer) {
    return world_renderer.tile_centroid_from_ij({ finish_tile_i, finish_tile_j });
}
LineSegment MovingEntity::home_target_segment(WorldRenderer& world_renderer) {

    Vector2D first_centroid = home_centroid(world_renderer);
    Vector2D second_centroid = target_centroid(world_renderer);

    return LineSegment(first_centroid.x, first_centroid.y, second_centroid.x, second_centroid.y);
}

LineSegment MovingEntity::move_delta_segment() {

    return LineSegment(prev_centroid.x, prev_centroid.y, centroid.x, centroid.y);
}

void MovingEntity::set_home(IJ home) {
    //TODO: post refactoring check equality works
    if (!(tile_ij.i == home.i && tile_ij.j == home.j)) {
        tile_ij = home;
    }
}
IJ MovingEntity::get_home_ij() const {
    return tile_ij;
}

void MovingEntity::set_home_and_center(IJ home, WorldRenderer& world_renderer) {

    set_home(home);

    //centroid = { 0,0 };
    centroid = world_renderer.tile_centroid_from_ij(tile_ij);
    prev_centroid = centroid;

    speed_vector = { 0, 0 };
}
void MovingEntity::set_target(IJ target, WorldRenderer& world_renderer) {

    finish_tile_i = target.i;
    finish_tile_j = target.j;

    Vector2D desired_velocity = world_renderer.tile_centroid_from_ij({ finish_tile_i, finish_tile_j }) - world_renderer.tile_centroid_from_ij({ tile_ij.i, tile_ij.j });

    desired_velocity.normalize();
    desired_velocity *= max_speed;

    speed_vector = desired_velocity;
}

bool MovingEntity::reached_target(WorldRenderer& world_renderer) {
    Vector2D target_vector = world_renderer.tile_centroid_from_ij({ finish_tile_i, finish_tile_j });

    double distance = target_vector.distance(centroid);

    return distance < 2; //TODO: this really depends on FPS
}

void MovingEntity::update(sf::Time& deltaTime, WorldRenderer& world_renderer) {
    prev_centroid = centroid;

    centroid += speed_vector * deltaTime.asSeconds();
}

void MovingEntity::draw(WorldRenderer& world_renderer) {
    Vector2D screen_point = world_renderer.tm * centroid;

    world_renderer.draw_circle(
        screen_point.x,
        screen_point.y,
        5.f,
        sf::Color::Red, sf::Color::Black
    );
}