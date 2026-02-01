#pragma once
#include "UIState.h"
class DummyCommandState : public UIState {
public:
    DummyCommandState(StateMachine<UIState, UI_InputEvent>* state_machine, GameWorld& game_world, WorldRenderer& world_renderer, sf::RenderWindow& window)
        : UIState(state_machine, game_world, world_renderer, window) {
    }
    void on_enter() noexcept {};
    void on_execute(UI_InputEvent event) noexcept {};
    void on_exit() noexcept {};
};
