#pragma once
#include <SFML/Graphics.hpp>

class WorldRenderer;
//TODO: convert to drawable shape
class CellCube {

    float cur_cell_hw;
    float cur_cell_hh;
    float cur_cell_height;

    std::array<sf::Vector2f, 8> cubeVertices;//TODO: remove sf:: dependency

    // Edges: pairs of indices
    std::array<std::pair<int, int>, 12> edges;//TODO: change to more meaningfull pair

    sf::Color _color;

public:
    CellCube(float cur_cell_hw, float cur_cell_hh, float cur_cell_height, sf::Color color);

    void draw(WorldRenderer& world_renderer, sf::Vector2f cube_pos) const;
};