#include "UnitCommandExecutionState.h"
#include "StateMachine.h"
#include "WorldRenderer.h"
#include "GameWorld.h"
#include "Unit.h"
#include <SFML/Graphics.hpp>

UnitCommandExecutionState::UnitCommandExecutionState(
    StateMachine<UIState, UI_InputEvent>* state_machine,
    GameWorld& game_world,
    WorldRenderer& world_renderer,
    Unit* unit
)
    : UIState(state_machine, game_world, world_renderer),
    _unit{ *unit },
    ui_cell_attack{ world_renderer.hw, world_renderer.hh, world_renderer.cell_height, sf::Color::Red }
{
}

void UnitCommandExecutionState::on_enter() noexcept {
    if (game_world.is_paused()) {
        game_world.unpause();
    }
}

void UnitCommandExecutionState::process_event(const UI_InputEvent& event) noexcept {

    auto mouse_ij = world_renderer.tile_ij_from_screen_xy(event.mouse_position);

    const XY<float> tile_xy = world_renderer.tile_screen_xy(mouse_ij);

    ui_cell_attack.draw(world_renderer, sf::Vector2f{ tile_xy.x, tile_xy.y });

    game_world.update(world_renderer); //TODO: this should probably move up
}

void UnitCommandExecutionState::on_exit() noexcept {
    if (!game_world.is_paused()) {
        game_world.pause();
    }
}