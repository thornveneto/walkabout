#include "Cell.h"
#include <stdexcept>
#include "../entities/Unit.h"
#include "Wall.h"
#include "../WorldRenderer.h"
#include "../math/LineSegment.h"

/*
* Utility function for cell instantiation
*/
sf::Color fill_from_code(CellCode cell_code) {
    if (cell_code == CellCode::GRASS) {
        return sf::Color{ 63, 155, 11 };
    }
    else if (cell_code == CellCode::WATER) {
        return sf::Color{ 45, 137, 239 };
    }

    return sf::Color::White;
}

/*
* Utility function for cell instantiation
*/
sf::Color border_from_code(CellCode cell_code) {
    if (cell_code == CellCode::GRASS) {
        return sf::Color{ 18, 57, 1 };
    }
    else if (cell_code == CellCode::WATER) {
        return sf::Color{ 11, 62, 117 };
    }

    return sf::Color::Black;
}

//TODO: there's a method for it, or we need a builder
Cell::Cell(
    CellCode cell_code,
    int i, int j,
    std::unique_ptr<Wall> north_slot,
    std::unique_ptr<Wall> east_slot,
    std::unique_ptr<Wall> south_slot,
    std::unique_ptr<Wall> west_slot,
    WorldRenderer* world_renderer
) :
    _cell_code{ cell_code }, _i{ i }, _j{ j },
    _north_slot{ std::move(north_slot) },
    _east_slot{ std::move(east_slot) },
    _south_slot{ std::move(south_slot) },
    _west_slot{ std::move(west_slot) },
    //TODO: this is absolutely horrible
    tile_drawing{ world_renderer->tile_screen_xy({ i,j }), fill_from_code(cell_code), border_from_code(cell_code), *world_renderer },
    world_renderer{ world_renderer }

{
    if (world_renderer == nullptr) {
        throw std::invalid_argument("World renderer cannot be null");
    }

    if (_north_slot && _north_slot->wall_orientation != WallOrientation::NORTH) {
        throw std::invalid_argument("North slot tile is not NORTH oriented");
    }
    if (_east_slot && _east_slot->wall_orientation != WallOrientation::EAST) {
        throw std::invalid_argument("East slot tile is not EAST oriented");
    }
    if (_south_slot && _south_slot->wall_orientation != WallOrientation::SOUTH) {
        throw std::invalid_argument("South slot tile is not EAST oriented");
    }
    if (_west_slot && _west_slot->wall_orientation != WallOrientation::WEST) {
        throw std::invalid_argument("West slot tile is not WEST oriented");
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
    _west_slot(std::move(other._west_slot)),

    //TODO: this is absolutely horrible
    tile_drawing{ world_renderer->tile_screen_xy({ other._i,other._i }), fill_from_code(other._cell_code), border_from_code(other._cell_code), *world_renderer },
    world_renderer(other.world_renderer)
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
        tile_drawing = std::move(other.tile_drawing);
        world_renderer = other.world_renderer;
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

LineSegment Cell::north_slot_segment() const {
    Vector2D tile_centroid = world_renderer->tile_centroid_from_ij({ _i, _j });

    return LineSegment(
        tile_centroid.x - world_renderer->tile_side / 2, tile_centroid.y - world_renderer->tile_side / 2,
        tile_centroid.x + world_renderer->tile_side / 2, tile_centroid.y - world_renderer->tile_side / 2
    );
}
LineSegment Cell::east_slot_segment() const {
    Vector2D tile_centroid = world_renderer->tile_centroid_from_ij({ _i, _j });

    return LineSegment(
        tile_centroid.x + world_renderer->tile_side / 2, tile_centroid.y - world_renderer->tile_side / 2,
        tile_centroid.x + world_renderer->tile_side / 2, tile_centroid.y + world_renderer->tile_side / 2
    );
}
LineSegment Cell::south_slot_segment() const {
    Vector2D tile_centroid = world_renderer->tile_centroid_from_ij({ _i, _j });

    return LineSegment(
        tile_centroid.x - world_renderer->tile_side / 2, tile_centroid.y + world_renderer->tile_side / 2,
        tile_centroid.x + world_renderer->tile_side / 2, tile_centroid.y + world_renderer->tile_side / 2
    );
}
LineSegment Cell::west_slot_segment() const {
    Vector2D tile_centroid = world_renderer->tile_centroid_from_ij({ _i, _j });

    return LineSegment(
        tile_centroid.x - world_renderer->tile_side / 2, tile_centroid.y - world_renderer->tile_side / 2,
        tile_centroid.x - world_renderer->tile_side / 2, tile_centroid.y + world_renderer->tile_side / 2
    );
}

void Cell::draw_tile(){

    tile_drawing.draw(*world_renderer);
}

void Cell::draw_cell_walls() const {
    if (_north_slot != nullptr) {
        _north_slot->draw(_i, _j, *world_renderer);
    }
    if (_east_slot != nullptr) {
        _east_slot->draw(_i, _j, *world_renderer);
    }
    if (_south_slot != nullptr) {
        _south_slot->draw(_i, _j, *world_renderer);
    }
    if (_west_slot != nullptr) {
        _west_slot->draw(_i, _j, *world_renderer);
    }
}

void Cell::add_guest(Unit* guest_entity) {
    _guests_set.insert(guest_entity);
}
void Cell::remove_guest(Unit* guest_entity) {
    _guests_set.erase(guest_entity);
}

Unit* Cell::get_unit() {
    //TODO: actually, there can only be one unit guest. But for now, dead units aren't counted as units

    Unit* result{ nullptr };

    for (auto guest_unit : _guests_set) {
        if (guest_unit->is_alive()) {
            result = guest_unit;
            break;
        }
    }

    return result;
}