#pragma once
#include "Cell.h"
#include <vector>
#include "IJ.h"

class WorldRenderer;
class Unit;
class Vector2D;

class Terrain {
    std::vector<std::vector<Cell>> terrain;
public:

    void init();

    std::vector<IJ> terrain_collisions(const LineSegment& entity_move_line_segment, WorldRenderer& world_renderer);

    Unit* unit_collision(const LineSegment& entity_move_line_segment, WorldRenderer& world_renderer);

    Cell& cell_at(IJ cell_ij);

    bool within_boundaries(const IJ& cell_ij) const;

    bool within_boundaries(const Vector2D& centroid, WorldRenderer& world_renderer) const;

    void transfer_guest_unit(IJ from, IJ to, Unit* guest);

    void draw(WorldRenderer& world_renderer);

    Unit* get_orderable_unit_at(IJ cell_ij);
};