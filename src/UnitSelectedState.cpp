#include "UnitSelectedState.h"
#include "StateMachine.h"
#include "WorldRenderer.h"
#include "GameWorld.h"
#include "Unit.h"
#include "UnitSelectionState.h"
#include "UnitCommandExecutionState.h"
#include <SFML/Graphics.hpp>

UnitSelectedState::UnitSelectedState(
    StateMachine<UIState, UI_InputEvent>* state_machine,
    GameWorld& game_world,
    WorldRenderer& world_renderer,
    Unit* unit
)
    : UIState(state_machine, game_world, world_renderer, unit),
    ui_cell_attack{ world_renderer.hw, world_renderer.hh, world_renderer.cell_height, sf::Color::Red }
{
}

void UnitSelectedState::on_enter() noexcept {}

void UnitSelectedState::process_event(const UI_InputEvent& event) noexcept {

    auto mouse_cell_ij = world_renderer.tile_ij_from_screen_xy(event.mouse_position);

    if (game_world.terrain.within_boundaries(mouse_cell_ij)) {

        const XY<float> tile_xy = world_renderer.tile_screen_xy(mouse_cell_ij);

        ui_cell_attack.draw(world_renderer, sf::Vector2f{ tile_xy.x, tile_xy.y }/*TODO: dependency*/);

        if (event.left_key_pressed) {

            Unit* unit = game_world.terrain.get_orderable_unit_at(mouse_cell_ij);

            if (unit) {
                _unit->shoot_at(mouse_cell_ij, world_renderer);
            }
            else {
                std::vector<IJ> path = game_world.terrain.find_path(_unit->get_home_ij(), mouse_cell_ij);

                _unit->set_waypoints(path);
                _unit->start_waypoints_following(world_renderer);
            }

            state_machine->switch_state(
                std::make_unique<UnitCommandExecutionState>(state_machine, game_world, world_renderer, _unit)
            );
        }
        else if (event.right_key_pressed) {

            _unit->deselect();

            state_machine->switch_state(
                std::make_unique<UnitSelectionState>(state_machine, game_world, world_renderer, nullptr)
            );
        }
    }
}

void UnitSelectedState::on_exit() noexcept {}