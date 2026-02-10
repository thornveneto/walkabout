#include "UnitSelectedState.h"
#include "StateMachine.h"
#include "WorldRenderer.h"
#include "GameWorld.h"
#include "Unit.h"
#include "UnitSelectionState.h"
#include "UnitCommandExecutionState.h"

UnitSelectedState::UnitSelectedState(
    StateMachine<UIState, UI_InputEvent>* state_machine,
    GameWorld& game_world,
    WorldRenderer& world_renderer,
    sf::RenderWindow& window,
    Unit* unit
)
    : UIState(state_machine, game_world, world_renderer, window),
    _unit{ *unit },
    ui_cell_attack{ world_renderer.hw, world_renderer.hh, world_renderer.cell_height, sf::Color::Red }
{
}

void UnitSelectedState::on_enter() noexcept {}

void UnitSelectedState::process_event(const UI_InputEvent& event) noexcept {

    auto mouse_ij = world_renderer.tile_ij_from_screen_xy(event.mouse_position);

    const XY<float> tile_xy = world_renderer.tile_screen_xy(mouse_ij);

    ui_cell_attack.draw(window, sf::Vector2f{ tile_xy.x, tile_xy.y });

    if (event.left_key_pressed) {

        _unit.shoot_at(mouse_ij, world_renderer);

        state_machine->switch_state(
            std::make_unique<UnitCommandExecutionState>(state_machine, game_world, world_renderer, window, _unit)
        );
    }
    else if (event.right_key_pressed) {

        _unit.deselect();

        state_machine->switch_state(
            std::make_unique<UnitSelectionState>(state_machine, game_world, world_renderer, window)
        );
    }
}

void UnitSelectedState::on_exit() noexcept {}