#include "UnitCommandExecutionState.h"
#include "Warrior.h"

void UnitCommandExecutionState::on_enter() noexcept {}

void UnitCommandExecutionState::on_execute() noexcept {

    auto mouse_ij = world_renderer.tile_ij_from_screen_xy(_ui_input_event.mouse_position.x, _ui_input_event.mouse_position.y);

    const XY<float> tile_xy = world_renderer.tile_screen_xy(mouse_ij);

    ui_cell_attack.draw(window, sf::Vector2f{ tile_xy.x, tile_xy.y });

    if (_ui_input_event.left_key_pressed) {

        Warrior* derrived_warrior = dynamic_cast<Warrior*>(&_orderable_entity);

        derrived_warrior->shoot_at(mouse_ij, world_renderer);

        //TODO: trace why is it moving passed.
        //_orderable_entity.set_target(mouse_ij, world_renderer);
    }

    game_world.update(world_renderer);

}

void UnitCommandExecutionState::on_exit() noexcept {}