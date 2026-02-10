#pragma once
#include <SFML/Graphics.hpp>
class CellAttackCube {

    float cur_cell_hw;
    float cur_cell_hh;
    float cur_cell_height;

    std::array<sf::Vector2f, 8> cubeVertices;

    // Edges: pairs of indices
    std::array<std::pair<int, int>, 12> edges;

    sf::Color _color;

public:
    CellAttackCube(float cur_cell_hw, float cur_cell_hh, float cur_cell_height, sf::Color color);

    void draw(sf::RenderWindow& window, sf::Vector2f cube_pos) const;
};