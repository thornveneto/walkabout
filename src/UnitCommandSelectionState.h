#pragma once
#include "UIState.h"
#include "CellCube.h"
class UnitCommandSelectionState : public UIState {
    CellCube ui_cell_selection;
public:
    UnitCommandSelectionState(StateMachine<UIState>* state_machine, GameWorld& game_world, WorldRenderer& world_renderer, sf::RenderWindow& window) :
        UIState(state_machine, game_world, world_renderer, window),  
        ui_cell_selection{ world_renderer.hw, world_renderer.hh, world_renderer.cell_height, sf::Color::Cyan }
    {
    }

    void on_enter() noexcept override;

    void on_execute() noexcept override;

    void on_exit() noexcept override;
};