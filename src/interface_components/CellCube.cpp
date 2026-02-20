#include "CellCube.h"
#include "../WorldRenderer.h"

CellCube::CellCube(float cur_cell_hw, float cur_cell_hh, float cur_cell_height, sf::Color color) :
    cur_cell_hw{ cur_cell_hw }, cur_cell_hh{ cur_cell_hh }, cur_cell_height{ cur_cell_height }, _color{ color } {

    cubeVertices = {
        sf::Vector2f{-cur_cell_hw,0}, sf::Vector2f{0,-cur_cell_hh},
        sf::Vector2f{cur_cell_hw,0},   sf::Vector2f{0,+cur_cell_hh},

        sf::Vector2f{-cur_cell_hw,0 - cur_cell_height}, sf::Vector2f{0,-cur_cell_hh - cur_cell_height},
        sf::Vector2f{cur_cell_hw,0 - cur_cell_height},   sf::Vector2f{0,+cur_cell_hh - cur_cell_height}
    };

    // Edges: pairs of indices
    edges = { {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    } };
}

void CellCube::draw(WorldRenderer& world_renderer, sf::Vector2f cube_pos) const {
    // Create VertexArray for edges
    sf::VertexArray lines(sf::PrimitiveType::Lines, edges.size() * 2);
    int idx = 0;
    for (auto& edge : edges) {
        sf::Vector2f p1 = cubeVertices[edge.first] + cube_pos;
        sf::Vector2f p2 = cubeVertices[edge.second] + cube_pos;

        sf::Vertex v1;
        v1.position = p1;
        v1.color = _color;

        sf::Vertex v2;
        v2.position = p2;
        v2.color = _color;

        lines[idx++] = v1;
        lines[idx++] = v2;
    }

    world_renderer.draw(lines);
}