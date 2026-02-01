#include "Terrain.h"

void Terrain::init() {
    terrain.resize(10);

    for (int i = 0; i < 10; ++i) {
        terrain[i].reserve(10);
        for (int j = 0; j < 10; ++j) {
            if (j == 7) {
                terrain[i].emplace_back(CellCode::WATER, i, j);
            }
            else {
                terrain[i].emplace_back(CellCode::GRASS, i, j);
            }

        }
    }

    terrain[1][1] = Cell{
        CellCode::GRASS,
        1,1,
        std::make_unique<Wall>(WallOrientation::NORTH),
        nullptr, nullptr, nullptr
    };
    terrain[1][2] = Cell{
        CellCode::GRASS,
        1,2,
        nullptr, std::make_unique<Wall>(WallOrientation::EAST),nullptr, nullptr
    };
    terrain[2][2] = Cell{
        CellCode::GRASS,
        2,2,
        nullptr, std::make_unique<Wall>(WallOrientation::EAST),nullptr, nullptr
    };
    terrain[3][3] = Cell{
        CellCode::GRASS,
        3,3,
        nullptr, nullptr, std::make_unique<Wall>(WallOrientation::SOUTH), nullptr
    };
    terrain[4][4] = Cell{
        CellCode::GRASS,
        4,4,
        nullptr, nullptr, nullptr, std::make_unique<Wall>(WallOrientation::WEST)
    };
    terrain[7][2] = Cell{
        CellCode::GRASS,
        7,2,
        std::make_unique<Wall>(WallOrientation::NORTH), std::make_unique<Wall>(WallOrientation::EAST),
        std::make_unique<Wall>(WallOrientation::SOUTH), std::make_unique<Wall>(WallOrientation::WEST)
    };
}

std::vector<IJ> Terrain::terrain_collisions(const LineSegment& entity_move_line_segment, WorldRenderer& world_renderer) {
    //TODO: check and optimize colliding tiles

    std::vector<IJ> result;

    for (int i = 0; i < terrain.size(); ++i) {
        for (int j = 0; j < terrain[i].size(); ++j) {

            if (terrain[i][j].has_north_wall()) {
                LineSegment& checked_wall_segment = terrain[i][j].north_slot_segment(world_renderer);

                if (checked_wall_segment.intersects(entity_move_line_segment)) {
                    result.push_back({ i, j });
                }
            }
            if (terrain[i][j].has_east_wall()) {
                LineSegment& checked_wall_segment = terrain[i][j].east_slot_segment(world_renderer);

                if (checked_wall_segment.intersects(entity_move_line_segment)) {
                    result.push_back({ i, j });
                }
            }
            if (terrain[i][j].has_south_wall()) {
                LineSegment& checked_wall_segment = terrain[i][j].south_slot_segment(world_renderer);

                if (checked_wall_segment.intersects(entity_move_line_segment)) {
                    result.push_back({ i, j });
                }
            }
            if (terrain[i][j].has_west_wall()) {
                LineSegment& checked_wall_segment = terrain[i][j].west_slot_segment(world_renderer);

                if (checked_wall_segment.intersects(entity_move_line_segment)) {
                    result.push_back({ i, j });
                }
            }

        }
    }

    return result;
}

Cell& Terrain::cell_at(IJ cell_ij) {

    return terrain[cell_ij.i][cell_ij.j];
}
bool Terrain::within_boundaries(IJ& cell_ij) {
    return (0 <= cell_ij.i && cell_ij.i < terrain.size()) && (0 <= cell_ij.j && cell_ij.j < terrain[0].size());
}

void Terrain::transfer_guest(IJ from, IJ to, MovingEntity* guest) {
    if (!(from.i == to.i && from.j == to.j)) {

        if (within_boundaries(from)) {
            cell_at(from).remove_guest(guest);
        }

        if (within_boundaries(to)) {
            cell_at(to).add_guest(guest);
        }
    }
}

void Terrain::draw(WorldRenderer& world_renderer) {
    //Draw Stuff here
    const sf::Color grass_color{ 63, 155, 11 };
    const sf::Color grass_border_color{ 18, 57, 1 };

    const sf::Color water_color{ 45, 137, 239 };
    const sf::Color water_border_color{ 11, 62, 117 };


    for (int i = 0; i < terrain.size(); ++i) {
        for (int j = 0; j < terrain[0].size(); ++j) {
            terrain[i][j].draw_tile(world_renderer);


        }
    }

    for (int i = 0; i < terrain.size(); ++i) {
        for (int j = 0; j < terrain[0].size(); ++j) {
            terrain[i][j].draw_cell_walls(world_renderer);
        }
    }
}

MovingEntity* Terrain::get_orderable_entity_at(IJ cell_ij) {
    return cell_at(cell_ij).get_orderable_entity();
}