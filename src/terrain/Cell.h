#pragma once
#include <set>
#include <memory>
#include "TileDrawing.h"

class Unit;
class Wall;
class WorldRenderer;
class LineSegment;

enum class CellCode {
    GRASS,
    WATER
};
class Cell {
public:

    //TODO: there's a method for it, or we need a builder
    Cell(
        CellCode cell_code,
        int i, int j,
        std::unique_ptr<Wall> north_slot,
        std::unique_ptr<Wall> east_slot,
        std::unique_ptr<Wall> south_slot,
        std::unique_ptr<Wall> west_slot,
        WorldRenderer* world_renderer
    );

    Cell(Cell&& other) noexcept;                // Move Constructor
    Cell& operator=(Cell&& other) noexcept;     // Move Assignment

    Cell(const Cell&) = delete;                 // No Copying allowed
    Cell& operator=(const Cell&) = delete;      // No Copying allowed

    ~Cell();

    void add_guest(Unit* guest_entity);
    void remove_guest(Unit* guest_entity);
    Unit* get_unit();

    bool has_north_wall() const;
    bool has_east_wall() const;
    bool has_south_wall() const;
    bool has_west_wall() const;

    LineSegment north_slot_segment() const;
    LineSegment east_slot_segment() const;
    LineSegment south_slot_segment() const;
    LineSegment west_slot_segment() const;

    void draw_tile();

    void draw_cell_walls() const;
private:
    CellCode _cell_code;
    //TODO: do we really need i and j?
    std::set<Unit*> _guests_set;
    int _i;
    int _j;
    std::unique_ptr<Wall> _north_slot;
    std::unique_ptr<Wall> _east_slot;
    std::unique_ptr<Wall> _south_slot;
    std::unique_ptr<Wall> _west_slot;

    TileDrawing tile_drawing;

    WorldRenderer* world_renderer;//TODO: is there anyway we could do it with reference cleanly?
};