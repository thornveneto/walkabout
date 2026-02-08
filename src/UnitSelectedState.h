#pragma once
#include "UIState.h"
#include "MovingEntity.h"
#include "CellAttackCube.h"
class UnitSelectedState : public UIState {
    MovingEntity& _unit;
    CellAttackCube ui_cell_attack;
public:
    UnitSelectedState(
        StateMachine<UIState, UI_InputEvent>* state_machine,
        GameWorld& game_world, 
        WorldRenderer& world_renderer, 
        sf::RenderWindow& window, 
        MovingEntity* unit
    )
        :   UIState(state_machine, game_world, world_renderer, window), 
            _unit{*unit },
        ui_cell_attack{ world_renderer.hw, world_renderer.hh, world_renderer.cell_height, sf::Color::Red }
    {
    }

    void on_enter() noexcept override;

    void process_event(const UI_InputEvent& event) noexcept override;

    void on_exit() noexcept override;
};