#include "UnitCommandExecutionState.h"
#include "../StateMachine.h"
#include "../WorldRenderer.h"
#include "../GameWorld.h"
#include "../entities/Unit.h"
#include <SFML/Graphics.hpp>
#include "UnitSelectionState.h"

UnitCommandExecutionState::UnitCommandExecutionState(
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

void UnitCommandExecutionState::on_enter() noexcept {

    if (game_world.is_paused()) {
        game_world.unpause();
    }
}

void UnitCommandExecutionState::process_event(const UI_InputEvent& event) noexcept {

    //TODO: this is sooo not cool, but not sure how to make it better
    if (!game_world.any_more_updates()) {
        state_machine->switch_state(
            std::make_unique<UnitSelectionState>(state_machine, game_world, world_renderer, nullptr, command_queue)
        );
    }
}

void UnitCommandExecutionState::on_exit() noexcept {

    _unit->deselect();

    if (!game_world.is_paused()) {
        game_world.pause();
    }
}