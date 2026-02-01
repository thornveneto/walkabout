#pragma once
#include "UIState.h"
#include "MovingEntity.h"
#include "CellAttackCube.h"
class UnitCommandExecutionState : public UIState {
    MovingEntity& _orderable_entity;
    CellAttackCube ui_cell_attack;
public:
    UnitCommandExecutionState(
        StateMachine<UIState, UI_InputEvent>* state_machine,
        GameWorld& game_world, 
        WorldRenderer& world_renderer, 
        sf::RenderWindow& window, 
        MovingEntity* orderable_entity
    )
        :   UIState(state_machine, game_world, world_renderer, window), 
            _orderable_entity{*orderable_entity },
        ui_cell_attack{ world_renderer.hw, world_renderer.hh, world_renderer.cell_height, sf::Color::Red }
    {
    }

    void on_enter() noexcept override;

    void on_execute(UI_InputEvent event) noexcept override;

    void on_exit() noexcept override;
};