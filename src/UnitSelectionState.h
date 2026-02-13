#pragma once
#include "UIState.h"
#include "CellCube.h"
#include "UI_InputEvent.h"

template <typename T, typename E>
class StateMachine;

class WorldRenderer;
class GameWorld;

class UnitSelectionState : public UIState {
    CellCube ui_cell_selection;
public:
    UnitSelectionState(StateMachine<UIState, UI_InputEvent>* state_machine, GameWorld& game_world, WorldRenderer& world_renderer);

    void on_enter() noexcept override;

    void process_event(const UI_InputEvent& event) noexcept override;

    void on_exit() noexcept override;
};