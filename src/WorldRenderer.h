#pragma once
#include <vector>
#include <utility>
#include <cmath>   // for std::round

#include <SFML/Graphics.hpp>
#include "Vector2D.h"
#include "Matrix2D.h"
#include "CellCube.h"
#include "XY.h"
#include "IJ.h"
//TODO: it's growing two responsibilities, rendering and parameters
class WorldRenderer {
    sf::RenderWindow& _window;
    const float _start_x{ 200.f };
    const float _start_y{ 400.f };
public:
    //game world sizes
    const float tile_side{ 10.f };
    const float cell_height{ 30.f };

    //tile display sides
    const float hw{ 20.f };
    const float hh{ 10.f };

    Matrix2D tm{
        hw / (tile_side), hw / (tile_side),
        -hh / (tile_side), hh / (tile_side)
    };

    WorldRenderer(sf::RenderWindow& window) : _window{ window } {}

    Vector2D tile_centroid_from_ij(IJ tile_ij);

    IJ tile_ij_from_centroid(Vector2D centroid) const;

    void draw_tile(int i, int j, const sf::Color& fill_color, const sf::Color& outline_color);

    void draw_circle(float x, float y, float r, const sf::Color& fill_color, const sf::Color& outline_color);

    void draw_ellipse(float x, float y, float ra, float rb, const sf::Color& fill_color, const sf::Color& outline_color);
    
    XY<float> tile_screen_xy(IJ ij) const;

    void draw(const sf::Drawable& drawable);

    IJ tile_ij_from_screen_xy(XY<int> xy) const;
};