#include "MovingEntity.h"
#include "GameWorld.h"
#include "WorldRenderer.h"

MovingEntity::MovingEntity(IJ home_cell, WorldRenderer& world_renderer, GameWorld& game_world) :
    _game_world{ game_world }, _cell_ij{ home_cell }, _centroid{ world_renderer.tile_centroid_from_ij(home_cell) } {
    //TODO: a bit ugly and only because we need to know the tile size
}

MovingEntity::~MovingEntity() = default;

void MovingEntity::stop() {
    speed_vector = { 0,0 };
}

Vector2D MovingEntity::centroid() const {
    return _centroid;
}

Vector2D MovingEntity::calculate_screen_point(WorldRenderer& world_renderer) const {
   return world_renderer.tm * _centroid;
}

bool MovingEntity::is_stopped() const {
    //avoiding unnecessary class instantiation
    return speed_vector.x == 0 && speed_vector.y == 0;
}

void MovingEntity::set_waypoints(const std::vector<IJ>& new_path) {
    _waypoints = new_path;
}
void MovingEntity::start_waypoints_following(WorldRenderer& world_renderer) {
    _waypoint_id = 0;
    set_home_and_center(_waypoints[_waypoint_id % _waypoints.size()], world_renderer);

    ++_waypoint_id;
    set_target(_waypoints[_waypoint_id % _waypoints.size()], world_renderer);
}
void MovingEntity::execute_waypoint_logic(WorldRenderer& world_renderer) {
    //TODO: ho-ho-ho flesh out the logic
    if (_waypoints.size() > 0 && reached_target(world_renderer)) {

        ++_waypoint_id;

        if (_waypoint_id == _waypoints.size()) {
            set_home_and_center(_waypoints[_waypoint_id-1], world_renderer);
            _waypoints.resize(0);
            stop();
        }
        else {
            set_target(_waypoints[_waypoint_id], world_renderer);
        }
    }
}

Vector2D MovingEntity::home_centroid(WorldRenderer& world_renderer) {
    return world_renderer.tile_centroid_from_ij(_cell_ij);
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

    return LineSegment(_prev_centroid.x, _prev_centroid.y, _centroid.x, _centroid.y);
}

void MovingEntity::set_home(IJ home) {
    //TODO: post refactoring check equality works
    if (!(_cell_ij.i == home.i && _cell_ij.j == home.j)) {
        _cell_ij = home;
    }
}
IJ MovingEntity::get_home_ij() const {
    return _cell_ij;
}

void MovingEntity::set_home_and_center(IJ home, WorldRenderer& world_renderer) {

    set_home(home);

    //centroid = { 0,0 };
    _centroid = world_renderer.tile_centroid_from_ij(_cell_ij);
    _prev_centroid = _centroid;

    speed_vector = { 0, 0 };
}
void MovingEntity::set_target(IJ target, WorldRenderer& world_renderer) {

    finish_tile_i = target.i;
    finish_tile_j = target.j;

    Vector2D desired_velocity = world_renderer.tile_centroid_from_ij({ finish_tile_i, finish_tile_j }) - world_renderer.tile_centroid_from_ij(_cell_ij);

    desired_velocity.normalize();
    desired_velocity *= max_speed;

    speed_vector = desired_velocity;
}

bool MovingEntity::reached_target(WorldRenderer& world_renderer) {
    Vector2D target_vector = world_renderer.tile_centroid_from_ij({ finish_tile_i, finish_tile_j });

    double distance = target_vector.distance(_centroid);

    return distance < 0.5; //TODO: this really depends on FPS
}

void MovingEntity::update(sf::Time& deltaTime, WorldRenderer& world_renderer) {
    _prev_centroid = _centroid;

    _centroid += speed_vector * deltaTime.asSeconds();
}

void MovingEntity::draw(WorldRenderer& world_renderer) {
    Vector2D screen_point = calculate_screen_point(world_renderer);

    world_renderer.draw_circle(
        screen_point.x,
        screen_point.y,
        5.f,
        sf::Color::Red, sf::Color::Black
    );
}