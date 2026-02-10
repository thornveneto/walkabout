#pragma once
#include "UIState.h"
#include "CellAttackCube.h"
#include <SFML/Graphics.hpp>
#include "UI_InputEvent.h"

template <typename T, typename E>
class StateMachine;

class WorldRenderer;
class GameWorld;
class Unit;

class UnitCommandExecutionState : public UIState {
    Unit& _unit;
    CellAttackCube ui_cell_attack;
public:
    UnitCommandExecutionState(
        StateMachine<UIState, UI_InputEvent>* state_machine,
        GameWorld& game_world,
        WorldRenderer& world_renderer,
        sf::RenderWindow& window,
        Unit* unit
    );

    void on_enter() noexcept override;

    void process_event(const UI_InputEvent& event) noexcept override;

    void on_exit() noexcept override;
};