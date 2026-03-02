#pragma once
#include "Cell.h"
#include <vector>
#include "../math/IJ.h"

class WorldRenderer;
class Unit;
class Vector2D;

class Terrain {
public:
    Terrain(WorldRenderer& world_renderer);

    void init();

    std::vector<IJ> terrain_collisions(const LineSegment& entity_move_line_segment);

    Unit* unit_collision(const LineSegment& entity_move_line_segment);

    Cell& cell_at(const IJ& cell_ij);

    bool within_boundaries(const IJ& cell_ij) const;

    bool within_boundaries(const Vector2D& centroid) const;

    void transfer_guest_unit(IJ from, IJ to, Unit* guest);

    void draw();

    Unit* unit_at(const IJ& cell_ij);

    std::vector<IJ> find_path(const IJ& start_cell, const IJ& end_cell);
private:
    WorldRenderer& world_renderer;

    std::vector<std::vector<Cell>> terrain;

    bool wall_between(IJ first_cell, IJ second_cell);
};