#include "WorldRenderer.h"
#include "EllipseShape.h"

Vector2D WorldRenderer::tile_centroid_from_ij(IJ tile_ij) {
    return {
        tile_ij.j * tile_side,
        tile_ij.i * tile_side
    };
}

IJ WorldRenderer::tile_ij_from_centroid(Vector2D centroid) const {
    //WATCH OUT: x,y order is reverse to i,j, since i is row not col
    return {
        static_cast<int>((centroid.y + tile_side / 2) / tile_side),
        static_cast<int>((centroid.x + tile_side / 2) / tile_side)
    };
}

Vector2D WorldRenderer::calculate_screen_point(Vector2D centroid) const {

    return _tm * centroid;
}

sf::Vector2f WorldRenderer::project_point_to_sf(float x, float y) {
    return { _start_x + x, _start_y + y };
}

void WorldRenderer::draw_circle(float x, float y, float r, const sf::Color& fill_color, const sf::Color& outline_color) {
    sf::CircleShape circle(r); // Radius of 50 pixels

    circle.setFillColor(fill_color);

    //outline - negative thickness to extrude inside
    circle.setOutlineThickness(-1.f);
    circle.setOutlineColor(outline_color);
    circle.setOrigin({ r, r });
    //circle.setPosition({ _start_x + x, _start_y + y });
    circle.setPosition(project_point_to_sf(x,y));

    _window.draw(circle);
}

void WorldRenderer::draw_ellipse(float x, float y, float ra, float rb, const sf::Color& fill_color, const sf::Color& outline_color) {

    EllipseShape ellipse({ ra,rb });

    ellipse.setFillColor(fill_color);
    ellipse.setOutlineThickness(-1.f);
    ellipse.setOutlineColor(outline_color);
    ellipse.setOrigin({ ra,rb });
    //ellipse.setPosition({ _start_x + x, _start_y + y });
    ellipse.setPosition(project_point_to_sf(x, y));

    _window.draw(ellipse);
}

XY<float> WorldRenderer::tile_screen_xy(IJ ij) const {
    return {
        _start_x + ij.i * hw + hw * ij.j,
        _start_y + ij.i * hh - hh * ij.j
    };
}
IJ WorldRenderer::tile_ij_from_screen_xy(XY<int> xy) const {
    float x = xy.x - _start_x;
    float y = xy.y - _start_y;

    float j = (x / hw - y / hh) / 2;

    float i = y / hh + j;

    return { static_cast<int>(std::round(i)),static_cast<int>(std::round(j)) };
}

void WorldRenderer::draw(const sf::Drawable& drawable) {
    _window.draw(drawable);
}