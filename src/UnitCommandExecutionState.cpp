#include "UnitCommandExecutionState.h"
#include "StateMachine.h"
#include "WorldRenderer.h"
#include "GameWorld.h"
#include "Unit.h"
#include <SFML/Graphics.hpp>
#include "UnitSelectionState.h"

UnitCommandExecutionState::UnitCommandExecutionState(
    StateMachine<UIState, UI_InputEvent>* state_machine,
    GameWorld& game_world,
    WorldRenderer& world_renderer,
    Unit* unit
)
    : UIState(state_machine, game_world, world_renderer, unit),
    ui_cell_attack{ world_renderer.hw, world_renderer.hh, world_renderer.cell_height, sf::Color::Red }
{
}

void UnitCommandExecutionState::on_enter() noexcept {
    if (game_world.is_paused()) {
        game_world.unpause();
    }
}

void UnitCommandExecutionState::process_event(const UI_InputEvent& event) noexcept {

    auto mouse_cell_ij = world_renderer.tile_ij_from_screen_xy(event.mouse_position);

    if (game_world.terrain.within_boundaries(mouse_cell_ij)) {

        const XY<float> tile_xy = world_renderer.tile_screen_xy(mouse_cell_ij);

        ui_cell_attack.draw(world_renderer, sf::Vector2f{ tile_xy.x, tile_xy.y }); 
    }

    game_world.update(world_renderer); //TODO: this should probably move up

    if (!game_world.any_more_updates()) {
        state_machine->switch_state(
            std::make_unique<UnitSelectionState>(state_machine, game_world, world_renderer, nullptr)
        );
    }
}

void UnitCommandExecutionState::on_exit() noexcept {

    _unit->deselect();

    if (!game_world.is_paused()) {
        game_world.pause();
    }
}