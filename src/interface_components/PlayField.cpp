#include "PlayField.h"
#include "../entities/Unit.h"
#include "../effects/Effect.h"
#include "../entities/Projectile.h"
#include "../palette.h"

PlayField::PlayField(GameWorld& game_world, WorldRenderer& world_renderer) :
    m_game_world{ game_world }, m_world_renderer{ world_renderer },
    m_screen_area{ 50,50, 1000, 500 }
{
    m_border_rectangle.setPosition({ static_cast<float>(m_screen_area.x), static_cast<float>(m_screen_area.y) });
    m_border_rectangle.setSize({ static_cast<float>(m_screen_area.width), static_cast<float>(m_screen_area.height) });
    m_border_rectangle.setFillColor(sf::Color::Transparent);
    m_border_rectangle.setOutlineThickness(-2.f);
    m_border_rectangle.setOutlineColor(palette::GRAY);
}

void PlayField::draw() {
    m_world_renderer.draw(m_border_rectangle);

    m_game_world.terrain.draw();

    for (const auto& unit : m_game_world.units_map()) {
        unit.second->draw(m_world_renderer);
    }

    for (const auto& projectile : m_game_world.projectiles_map()) {
        projectile.second->draw(m_world_renderer);
    }

    for (auto& effect : m_game_world.effects()) {
        effect->draw(m_world_renderer);
    }
}