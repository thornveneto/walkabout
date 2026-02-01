#pragma once
#include "WorldRenderer.h"
#include "Wall.h"
#include "LineSegment.h"
#include <set>

//#include "MovingEntity.h"
class MovingEntity;

enum class CellCode {
    GRASS,
    WATER
};
class Cell {
    CellCode _cell_code;
    //TODO: do we really need i and j?
    std::set<MovingEntity*> _guests_set;
    int _i;
    int _j;
    std::unique_ptr<Wall> _north_slot;
    std::unique_ptr<Wall> _east_slot;
    std::unique_ptr<Wall> _south_slot;
    std::unique_ptr<Wall> _west_slot;
public:

    //TODO: there's a method for it, or we need a builder
    Cell(CellCode cell_code, int i, int j) : _cell_code{ cell_code }, _i{ i }, _j{ j } {}
    Cell(CellCode cell_code, int i, int j, 
        std::unique_ptr<Wall> north_slot, std::unique_ptr<Wall> east_slot, std::unique_ptr<Wall> south_slot, std::unique_ptr<Wall> west_slot
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

    void add_guest(MovingEntity* guest_entity);
    void remove_guest(MovingEntity* guest_entity);
    MovingEntity* get_orderable_entity();

    bool has_north_wall() const;
    bool has_east_wall() const;
    bool has_south_wall() const;
    bool has_west_wall() const;

    LineSegment north_slot_segment(WorldRenderer& world_renderer) const;
    LineSegment east_slot_segment(WorldRenderer& world_renderer) const;
    LineSegment south_slot_segment(WorldRenderer& world_renderer) const;
    LineSegment west_slot_segment(WorldRenderer& world_renderer) const;

    void draw_tile(WorldRenderer& world_renderer);

    void draw_cell_walls(WorldRenderer& world_renderer);
};