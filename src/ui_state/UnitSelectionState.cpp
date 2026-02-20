#include "UnitSelectionState.h"
#include "UnitSelectedState.h"
#include "../entities//Unit.h"
#include "../StateMachine.h"
#include "../GameWorld.h"
#include "../WorldRenderer.h"
#include <SFML/Graphics.hpp>

UnitSelectionState::UnitSelectionState(StateMachine<UIState, UI_InputEvent>* state_machine, GameWorld& game_world, WorldRenderer& world_renderer, Unit* unit,std::deque<GameCommand>& command_queue) :
UIState(state_machine, game_world, world_renderer, unit, command_queue),
ui_cell_selection{ world_renderer.hw, world_renderer.hh, world_renderer.cell_height, sf::Color::Cyan }
{
}

void UnitSelectionState::on_enter() noexcept {}

void UnitSelectionState::process_event(const UI_InputEvent& event) noexcept {
    auto mouse_cell_ij = world_renderer.tile_ij_from_screen_xy(event.mouse_position);

    if (game_world.terrain.within_boundaries(mouse_cell_ij)) {

        const XY<float> tile_xy = world_renderer.tile_screen_xy(mouse_cell_ij);

        if (event.left_key_pressed) {
            Unit* unit = game_world.terrain.get_orderable_unit_at(mouse_cell_ij);

            if (unit) {
                command_queue.push_back(GameCommand(CommandType::SELECT_UNIT, mouse_cell_ij, unit));
            }
        }
    }
}

void UnitSelectionState::on_exit() noexcept {}

void UnitSelectionState::draw(sf::RenderWindow& window) {
    sf::Vector2i mouse_position = sf::Mouse::getPosition(window);

    auto mouse_cell_ij = world_renderer.tile_ij_from_screen_xy({ mouse_position.x, mouse_position.y });

    if (game_world.terrain.within_boundaries(mouse_cell_ij)) {
        //TODO: same code as in process event and other classes
        const XY<float> tile_xy = world_renderer.tile_screen_xy(mouse_cell_ij);

        ui_cell_selection.draw(world_renderer, sf::Vector2f{ tile_xy.x, tile_xy.y }/*TODO: dependency*/);
    }
}