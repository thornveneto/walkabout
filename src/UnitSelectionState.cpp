#pragma once
#include "UnitSelectionState.h"
#include <memory>
#include "UnitSelectedState.h"

void UnitSelectionState::on_enter() noexcept {}

void UnitSelectionState::process_event(const UI_InputEvent& event) noexcept {
    auto mouse_ij = world_renderer.tile_ij_from_screen_xy(event.mouse_position.x, event.mouse_position.y);

    const XY<float> tile_xy = world_renderer.tile_screen_xy(mouse_ij);

    ui_cell_selection.draw(window, sf::Vector2f{ tile_xy.x, tile_xy.y });

    if (event.left_key_pressed) {
        MovingEntity* orderable_entity = game_world.terrain.get_orderable_entity_at(mouse_ij);

        if (orderable_entity) {
            state_machine->switch_state(
                std::make_unique<UnitSelectedState>(state_machine, game_world, world_renderer, window, orderable_entity)
            );
        }
    }
}

void UnitSelectionState::on_exit() noexcept {}