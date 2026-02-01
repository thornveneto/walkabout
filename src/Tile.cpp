#include "Tile.h"
bool Tile::has_north_wall() const {
    return _north_slot != nullptr;
}
bool Tile::has_east_wall() const {
    return _east_slot != nullptr;
}
bool Tile::has_south_wall() const {
    return _south_slot != nullptr;
}
bool Tile::has_west_wall() const {
    return _west_slot != nullptr;
}

LineSegment Tile::north_slot_segment(WorldRenderer& world_renderer) const {
    Vector2D tile_centroid = world_renderer.tile_centroid_from_ij({ _i, _j });

    return LineSegment(
        tile_centroid.x - world_renderer.tile_side / 2, tile_centroid.y - world_renderer.tile_side / 2,
        tile_centroid.x + world_renderer.tile_side / 2, tile_centroid.y - world_renderer.tile_side / 2
    );
}
LineSegment Tile::east_slot_segment(WorldRenderer& world_renderer) const {
    Vector2D tile_centroid = world_renderer.tile_centroid_from_ij({ _i, _j });

    return LineSegment(
        tile_centroid.x + world_renderer.tile_side / 2, tile_centroid.y - world_renderer.tile_side / 2,
        tile_centroid.x + world_renderer.tile_side / 2, tile_centroid.y + world_renderer.tile_side / 2
    );
}
LineSegment Tile::south_slot_segment(WorldRenderer& world_renderer) const {
    Vector2D tile_centroid = world_renderer.tile_centroid_from_ij({ _i, _j });

    return LineSegment(
        tile_centroid.x - world_renderer.tile_side / 2, tile_centroid.y + world_renderer.tile_side / 2,
        tile_centroid.x + world_renderer.tile_side / 2, tile_centroid.y + world_renderer.tile_side / 2
    );
}
LineSegment Tile::west_slot_segment(WorldRenderer& world_renderer) const {
    Vector2D tile_centroid = world_renderer.tile_centroid_from_ij({ _i, _j });

    return LineSegment(
        tile_centroid.x - world_renderer.tile_side / 2, tile_centroid.y - world_renderer.tile_side / 2,
        tile_centroid.x - world_renderer.tile_side / 2, tile_centroid.y + world_renderer.tile_side / 2
    );
}

void Tile::draw_tile(WorldRenderer& world_renderer) {
    //Draw Stuff here
    const sf::Color grass_color{ 63, 155, 11 };
    const sf::Color grass_border_color{ 18, 57, 1 };

    const sf::Color water_color{ 45, 137, 239 };
    const sf::Color water_border_color{ 11, 62, 117 };

    if (_tile_code == TileCode::GRASS) {
        world_renderer.draw_tile(
            _i, _j,
            grass_color, grass_border_color
        );
    }
    else if (_tile_code == TileCode::WATER) {
        world_renderer.draw_tile(
            _i, _j,
            water_color, water_border_color
        );
    }
}

void Tile::draw_tile_walls(WorldRenderer& world_renderer) {
    if (_north_slot != nullptr) {
        _north_slot->draw(_i, _j, world_renderer);
    }
    if (_east_slot != nullptr) {
        _east_slot->draw(_i, _j, world_renderer);
    }
    if (_south_slot != nullptr) {
        _south_slot->draw(_i, _j, world_renderer);
    }
    if (_west_slot != nullptr) {
        _west_slot->draw(_i, _j, world_renderer);
    }
}

void Tile::add_guest(MovingEntity* guest_entity) {
    _guests_set.insert(guest_entity);
}
void Tile::remove_guest(MovingEntity* guest_entity) {
    _guests_set.erase(guest_entity);
}

MovingEntity* Tile::get_orderable_entity() {

    MovingEntity* result{ nullptr };

    for (auto e : _guests_set) {
        std::cout << "FOUND ONE" << std::endl;
        result = e;
        break;
    }

    return result;
}