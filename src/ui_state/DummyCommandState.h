#pragma once
#include "UIState.h"
#include "../UI_InputEvent.h"

template <typename T, typename E>
class StateMachine;

class WorldRenderer;
class GameWorld;

class DummyCommandState : public UIState {
public:
    DummyCommandState(StateMachine<UIState, UI_InputEvent>* state_machine, GameWorld& game_world, WorldRenderer& world_renderer);
    void on_enter() noexcept {};
    void process_event(const UI_InputEvent& event) noexcept override {};
    void on_exit() noexcept {};
};
