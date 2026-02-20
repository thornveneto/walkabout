#include "UnitSelectedState.h"
#include "../StateMachine.h"
#include "../WorldRenderer.h"
#include "../GameWorld.h"
#include "../entities/Unit.h"
#include "UnitSelectionState.h"
#include "UnitCommandExecutionState.h"
#include <SFML/Graphics.hpp>

UnitSelectedState::UnitSelectedState(
    StateMachine<UIState, UI_InputEvent>* state_machine,
    GameWorld& game_world,
    WorldRenderer& world_renderer,
    Unit* unit,
    std::deque<GameCommand>& command_queue
)
    : UIState(state_machine, game_world, world_renderer, unit, command_queue),
    ui_cell_attack{ world_renderer.hw, world_renderer.hh, world_renderer.cell_height, sf::Color::Red }
{
}

void UnitSelectedState::on_enter() noexcept {}

void UnitSelectedState::process_event(const UI_InputEvent& event) noexcept {

    auto mouse_cell_ij = world_renderer.tile_ij_from_screen_xy(event.mouse_position);

    if (game_world.terrain.within_boundaries(mouse_cell_ij)) {

        const XY<float> tile_xy = world_renderer.tile_screen_xy(mouse_cell_ij);

        if (event.left_key_pressed) {

            Unit* unit = game_world.terrain.get_orderable_unit_at(mouse_cell_ij);

            if (unit) {
                command_queue.push_back(GameCommand(CommandType::ATTACK, mouse_cell_ij, _unit));
            }
            else {
                command_queue.push_back(GameCommand(CommandType::MOVE, mouse_cell_ij, _unit));
            }
        }
        else if (event.right_key_pressed) {

            command_queue.push_back(GameCommand(CommandType::DESELECT_UNIT, mouse_cell_ij, _unit));
        }
    }
}

void UnitSelectedState::draw(sf::RenderWindow& window) {
    sf::Vector2i mouse_position = sf::Mouse::getPosition(window);

    auto mouse_cell_ij = world_renderer.tile_ij_from_screen_xy({ mouse_position.x, mouse_position.y });

    if (game_world.terrain.within_boundaries(mouse_cell_ij)) {
        //TODO: same code as in process event and other classes

        const XY<float> tile_xy = world_renderer.tile_screen_xy(mouse_cell_ij);

        ui_cell_attack.draw(world_renderer, sf::Vector2f{ tile_xy.x, tile_xy.y }/*TODO: dependency*/);
    }
}

void UnitSelectedState::on_exit() noexcept {}