#pragma once
#include "UIState.h"
#include "../interface_components/CellAttackCube.h"
#include "../UI_InputEvent.h"

template <typename T, typename E>
class StateMachine;

class WorldRenderer;
class GameWorld;
class Unit;

class UnitCommandExecutionState : public UIState {
    CellAttackCube ui_cell_attack;
public:
    UnitCommandExecutionState(
        StateMachine<UIState, UI_InputEvent>* state_machine,
        GameWorld& game_world,
        WorldRenderer& world_renderer,
        Unit* unit,
        std::deque<GameCommand>& command_queue
    );

    void on_enter() noexcept override;

    void process_event(const UI_InputEvent& event) noexcept override;

    void on_exit() noexcept override;

    void draw(sf::RenderWindow& window) override {};
};