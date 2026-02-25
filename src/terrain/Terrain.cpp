#include "Terrain.h"
#include "../entities/Unit.h"
#include "Wall.h"
#include "../math/LineSegment.h"
#include "../WorldRenderer.h"
#include "../math/Vector2D.h"
#include <deque>
#include <map>

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

Unit* Terrain::unit_collision(const LineSegment& entity_move_line_segment, WorldRenderer& world_renderer) {
    //TODO: ugly prototype way

    Vector2D trajectory_centroid = entity_move_line_segment.end_vector();

    IJ cell_ij_to_inspect = world_renderer.tile_ij_from_centroid(trajectory_centroid);

    if (within_boundaries(cell_ij_to_inspect)) {
        //Safety measure to deal with unchecked out of bounds projectiles
        Cell& cell_to_inspect = cell_at(cell_ij_to_inspect);

        return cell_to_inspect.get_unit();
    }

    return nullptr;
}

std::vector<IJ> Terrain::terrain_collisions(const LineSegment& entity_move_line_segment, WorldRenderer& world_renderer) {
    //TODO: check and optimize colliding tiles

    std::vector<IJ> result;

    for (int i = 0; i < terrain.size(); ++i) {
        for (int j = 0; j < terrain[i].size(); ++j) {

            if (terrain[i][j].has_north_wall()) {
                LineSegment checked_wall_segment = terrain[i][j].north_slot_segment(world_renderer);

                if (checked_wall_segment.intersects(entity_move_line_segment)) {
                    result.push_back({ i, j });
                }
            }
            if (terrain[i][j].has_east_wall()) {
                LineSegment checked_wall_segment = terrain[i][j].east_slot_segment(world_renderer);

                if (checked_wall_segment.intersects(entity_move_line_segment)) {
                    result.push_back({ i, j });
                }
            }
            if (terrain[i][j].has_south_wall()) {
                LineSegment checked_wall_segment = terrain[i][j].south_slot_segment(world_renderer);

                if (checked_wall_segment.intersects(entity_move_line_segment)) {
                    result.push_back({ i, j });
                }
            }
            if (terrain[i][j].has_west_wall()) {
                LineSegment checked_wall_segment = terrain[i][j].west_slot_segment(world_renderer);

                if (checked_wall_segment.intersects(entity_move_line_segment)) {
                    result.push_back({ i, j });
                }
            }

        }
    }

    return result;
}

Cell& Terrain::cell_at(const IJ& cell_ij) {

    return terrain[cell_ij.i][cell_ij.j];
}
bool Terrain::within_boundaries(const IJ& cell_ij) const {
    return (0 <= cell_ij.i && cell_ij.i < terrain.size()) && (0 <= cell_ij.j && cell_ij.j < terrain[0].size());
}

bool Terrain::within_boundaries(const Vector2D& centroid, WorldRenderer& world_renderer) const {

    return within_boundaries(world_renderer.tile_ij_from_centroid(centroid));
}

void Terrain::transfer_guest_unit(IJ from, IJ to, Unit* guest) {
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

Unit* Terrain::get_orderable_unit_at(const IJ& cell_ij) {
    return cell_at(cell_ij).get_unit();
}

bool Terrain::wall_between(IJ first_cell_ij, IJ second_cell_ij) {

    //TODO: add validation that cells are adjacent in a line
    Cell& first_cell = cell_at(first_cell_ij);
    Cell& second_cell = cell_at(second_cell_ij);

    bool result{ false };
    if (!result && first_cell_ij.i + 1 == second_cell_ij.i) {
        result = first_cell.has_south_wall() || second_cell.has_north_wall();
    }

    if (!result && first_cell_ij.i - 1 == second_cell_ij.i) {
        result = first_cell.has_north_wall() || second_cell.has_south_wall();
    }

    if (!result && first_cell_ij.j+1 == second_cell_ij.j) {
        result = first_cell.has_east_wall() || second_cell.has_west_wall();
    }

    if (!result && first_cell_ij.j-1 == second_cell_ij.j) {
        result = first_cell.has_west_wall() || second_cell.has_east_wall();
    }

    return result;
}

std::vector<IJ> Terrain::find_path(const IJ& start_cell, const IJ& end_cell) {
    if (!within_boundaries(start_cell)) {
        throw std::invalid_argument("find_path.start cell must be within boundaries");
    }

    if(!within_boundaries(end_cell)) {
        throw std::invalid_argument("find_path.end cell must be within boundaries");
    }

    //TODO: move algorithm in a dedicated class
    std::deque<IJ> Q{ start_cell};
    std::set<IJ> V;
    std::map<IJ, IJ> path;

    while (Q.size() > 0) {
        IJ q = Q.front();
        Q.pop_front();

        if (q == end_cell) {
            break;
        }

        //TODO: can we make it cleaner?

        IJ south = { q.i + 1, q.j };
        if (within_boundaries(south) && V.count(south) == 0 && !wall_between(q, south) && !cell_at(south).get_unit()) {
            Q.push_back(south);
            path[south] = { q };
            V.insert(south);
        }

        IJ north = { q.i - 1, q.j };
        if (within_boundaries(north) && V.count(north) == 0 && !wall_between(q, north) && !cell_at(north).get_unit()) {
            Q.push_back(north);
            path[north] = { q };
            V.insert(north);
        }

        IJ east = { q.i, q.j + 1 };
        if (within_boundaries(east) && V.count(east) == 0 && !wall_between(q, east) && !cell_at(east).get_unit()) {
            Q.push_back(east);
            path[east] = { q };
            V.insert(east);
        }

        IJ west = { q.i, q.j - 1 };
        if (within_boundaries(west) && V.count(west) == 0 && !wall_between(q, west) && !cell_at(west).get_unit()) {
            Q.push_back(west);
            path[west] = { q };
            V.insert(west);
        }


        V.insert(q);
    }

    std::vector<IJ> result;
    IJ runner = end_cell;

    while(!(runner == start_cell)) {
        result.push_back(runner);

        runner = path.at(runner);
    }

    result.push_back(runner);

    std::reverse(result.begin(), result.end());

    return result;
}