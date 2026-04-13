#include "PlayField.h"
#include "../entities/Unit.h"
#include "../effects/Effect.h"
#include "../entities/Projectile.h"
#include "../palette.h"
#include "../entities/Weapon.h"

PlayField::PlayField(GameWorld& game_world, WorldRenderer& world_renderer, std::deque<GameCommand>& command_queue) :
    m_game_world{ game_world }, m_world_renderer{ world_renderer }, m_command_queue{command_queue},
    m_screen_area{ 50,50, 1000, 500 }
{
    m_border_rectangle.setPosition({ static_cast<float>(m_screen_area.x), static_cast<float>(m_screen_area.y) });
    m_border_rectangle.setSize({ static_cast<float>(m_screen_area.width), static_cast<float>(m_screen_area.height) });
    m_border_rectangle.setFillColor(sf::Color::Transparent);
    m_border_rectangle.setOutlineThickness(-2.f);
    m_border_rectangle.setOutlineColor(palette::GRAY);
}

void PlayField::handle_event(UI_InputEvent& event, GameStateDesc& game_state_desc) {
    if ((event.left_key_pressed || event.right_key_pressed ) && m_screen_area.within(event.mouse_position)) {
        std::cout << "PlayField::handle_event -> Button clicked" << std::endl;

        auto mouse_cell_ij = m_world_renderer.tile_ij_from_screen_xy(event.mouse_position);

        if (m_game_world.terrain.within_boundaries(mouse_cell_ij)) {

            std::cout << "PlayField::handle_event -> Within boundaries" << std::endl;

            const XY<float> tile_xy = m_world_renderer.tile_screen_xy(mouse_cell_ij);

            if (event.left_key_pressed) {
                Unit* clicked_unit = m_game_world.terrain.unit_at(mouse_cell_ij);

                if (clicked_unit && game_state_desc.control_mode == ControlMode::UnitSelection) {
                    std::cout << "PlayField::handle_event -> clicked_unit AND ControlMode::UnitSelection" << std::endl;

                    std::cout << "PlayField::handle_event -> emit SELECT_UNIT" << std::endl;

                    //TODO: move this to interpreter
                    m_command_queue.push_back(GameCommand(CommandType::SELECT_UNIT, mouse_cell_ij, clicked_unit));
                } else if (clicked_unit && game_state_desc.control_mode == ControlMode::UnitSelected && clicked_unit != game_state_desc.active_unit) {
                    std::cout << "PlayField::handle_event -> clicked_unit AND ControlMode::UnitSelected AND clicked_unit != active_unit" << std::endl;

                    std::cout << "PlayField::handle_event -> emit ATTACK" << std::endl;

                    m_command_queue.push_back(GameCommand(CommandType::ATTACK, mouse_cell_ij, game_state_desc.active_unit));
                } else if (!clicked_unit && game_state_desc.control_mode == ControlMode::UnitSelected) {
                    std::cout << "PlayField::handle_event -> NOT clicked_unit AND ControlMode::UnitSelected" << std::endl;

                    std::cout << "PlayField::handle_event -> emit MOVE" << std::endl;

                    m_command_queue.push_back(GameCommand(CommandType::MOVE, mouse_cell_ij, game_state_desc.active_unit));
                }
                else if (clicked_unit && game_state_desc.control_mode == ControlMode::UnitSelected && clicked_unit == game_state_desc.active_unit) {
                    std::cout << "PlayField::handle_event -> clicked_unit AND ControlMode::UnitSelected AND clicked_unit == active_unit" << std::endl;

                    std::cout << "PlayField::handle_event -> emit DESELECT" << std::endl;

                    m_command_queue.push_back(GameCommand(CommandType::DESELECT_UNIT, mouse_cell_ij, game_state_desc.active_unit));
                }
                else {
                    std::cout << "PlayField::handle_event -> no left_key_pressed state handler" << std::endl;
                }
            }
            else if (event.right_key_pressed && game_state_desc.control_mode == ControlMode::UnitSelected/*TODO: we don't check invariant anywhere*/) {

                m_command_queue.push_back(GameCommand(CommandType::DESELECT_UNIT, mouse_cell_ij, game_state_desc.active_unit));
            }
        }
    }
}

void PlayField::draw(sf::RenderWindow& window, GameStateDesc& game_state_desc) {
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