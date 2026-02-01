#pragma once
#include "WorldRenderer.h"
#include "Tile.h"
#include "Wall.h"
#include "IJ.h"
class Terrain {
    std::vector<std::vector<Tile>> terrain;
public:

    void init();

    std::vector<IJ> terrain_collisions(const LineSegment& entity_move_line_segment, WorldRenderer& world_renderer);

    Tile& tile_at(IJ tile_ij);

    bool within_boundaries(IJ& tile_ij);

    void transfer_guest(IJ from, IJ to, MovingEntity* guest);

    void draw(WorldRenderer& world_renderer);

    MovingEntity* get_orderable_entity_at(IJ tile_ij);
};