#include "Cell.h"
#include <stdexcept>
#include "Unit.h"
#include "Wall.h"
#include "WorldRenderer.h"
#include "LineSegment.h"

//TODO: there's a method for it, or we need a builder
Cell::Cell(CellCode cell_code, int i, int j) : _cell_code{ cell_code }, _i{ i }, _j{ j } {}
Cell::Cell(
    CellCode cell_code,
    int i, int j,
    std::unique_ptr<Wall> north_slot,
    std::unique_ptr<Wall> east_slot,
    std::unique_ptr<Wall> south_slot,
    std::unique_ptr<Wall> west_slot
) :
    _cell_code{ cell_code }, _i{ i }, _j{ j },
    _north_slot{ std::move(north_slot) },
    _east_slot{ std::move(east_slot) },
    _south_slot{ std::move(south_slot) },
    _west_slot{ std::move(west_slot) }
{
    if (_north_slot && _north_slot->wall_orientation != WallOrientation::NORTH) {
        throw std::runtime_error("North slot tile is not NORTH oriented");
    }
    if (_east_slot && _east_slot->wall_orientation != WallOrientation::EAST) {
        throw std::runtime_error("East slot tile is not EAST oriented");
    }
    if (_south_slot && _south_slot->wall_orientation != WallOrientation::SOUTH) {
        throw std::runtime_error("South slot tile is not EAST oriented");
    }
    if (_west_slot && _west_slot->wall_orientation != WallOrientation::WEST) {
        throw std::runtime_error("West slot tile is not WEST oriented");
    }
}

// Move Constructor implementation
Cell::Cell(Cell&& other) noexcept
    : _cell_code(other._cell_code),
    _guests_set(std::move(other._guests_set)),
    _i(other._i),
    _j(other._j),
    _north_slot(std::move(other._north_slot)),
    _east_slot(std::move(other._east_slot)),
    _south_slot(std::move(other._south_slot)),
    _west_slot(std::move(other._west_slot))
{
}

// Move Assignment implementation
Cell& Cell::operator=(Cell&& other) noexcept {
    if (this != &other) {
        _cell_code = other._cell_code;
        _guests_set = std::move(other._guests_set);
        _i = other._i;
        _j = other._j;
        _north_slot = std::move(other._north_slot);
        _east_slot = std::move(other._east_slot);
        _south_slot = std::move(other._south_slot);
        _west_slot = std::move(other._west_slot);
    }
    return *this;
}


Cell::~Cell() = default;

bool Cell::has_north_wall() const {
    return _north_slot != nullptr;
}
bool Cell::has_east_wall() const {
    return _east_slot != nullptr;
}
bool Cell::has_south_wall() const {
    return _south_slot != nullptr;
}
bool Cell::has_west_wall() const {
    return _west_slot != nullptr;
}

LineSegment Cell::north_slot_segment(WorldRenderer& world_renderer) const {
    Vector2D tile_centroid = world_renderer.tile_centroid_from_ij({ _i, _j });

    return LineSegment(
        tile_centroid.x - world_renderer.tile_side / 2, tile_centroid.y - world_renderer.tile_side / 2,
        tile_centroid.x + world_renderer.tile_side / 2, tile_centroid.y - world_renderer.tile_side / 2
    );
}
LineSegment Cell::east_slot_segment(WorldRenderer& world_renderer) const {
    Vector2D tile_centroid = world_renderer.tile_centroid_from_ij({ _i, _j });

    return LineSegment(
        tile_centroid.x + world_renderer.tile_side / 2, tile_centroid.y - world_renderer.tile_side / 2,
        tile_centroid.x + world_renderer.tile_side / 2, tile_centroid.y + world_renderer.tile_side / 2
    );
}
LineSegment Cell::south_slot_segment(WorldRenderer& world_renderer) const {
    Vector2D tile_centroid = world_renderer.tile_centroid_from_ij({ _i, _j });

    return LineSegment(
        tile_centroid.x - world_renderer.tile_side / 2, tile_centroid.y + world_renderer.tile_side / 2,
        tile_centroid.x + world_renderer.tile_side / 2, tile_centroid.y + world_renderer.tile_side / 2
    );
}
LineSegment Cell::west_slot_segment(WorldRenderer& world_renderer) const {
    Vector2D tile_centroid = world_renderer.tile_centroid_from_ij({ _i, _j });

    return LineSegment(
        tile_centroid.x - world_renderer.tile_side / 2, tile_centroid.y - world_renderer.tile_side / 2,
        tile_centroid.x - world_renderer.tile_side / 2, tile_centroid.y + world_renderer.tile_side / 2
    );
}

void Cell::draw_tile(WorldRenderer& world_renderer) const {
    //Draw Stuff here
    const sf::Color grass_color{ 63, 155, 11 };
    const sf::Color grass_border_color{ 18, 57, 1 };

    const sf::Color water_color{ 45, 137, 239 };
    const sf::Color water_border_color{ 11, 62, 117 };

    if (_cell_code == CellCode::GRASS) {
        world_renderer.draw_tile(
            _i, _j,
            grass_color, grass_border_color
        );
    }
    else if (_cell_code == CellCode::WATER) {
        world_renderer.draw_tile(
            _i, _j,
            water_color, water_border_color
        );
    }
}

void Cell::draw_cell_walls(WorldRenderer& world_renderer) const {
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

void Cell::add_guest(Unit* guest_entity) {
    _guests_set.insert(guest_entity);
}
void Cell::remove_guest(Unit* guest_entity) {
    _guests_set.erase(guest_entity);
}

Unit* Cell::get_orderable_unit() {

    Unit* result{ nullptr };

    for (auto e : _guests_set) {
        result = e;
        break;
    }

    return result;
}