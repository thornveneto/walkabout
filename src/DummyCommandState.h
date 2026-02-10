#pragma once
#include "UIState.h"
#include "UI_InputEvent.h"
#include <SFML/Graphics.hpp>

template <typename T, typename E>
class StateMachine;

class WorldRenderer;
class GameWorld;

class DummyCommandState : public UIState {
public:
    DummyCommandState(StateMachine<UIState, UI_InputEvent>* state_machine, GameWorld& game_world, WorldRenderer& world_renderer, sf::RenderWindow& window);
    void on_enter() noexcept {};
    void process_event(const UI_InputEvent& event) noexcept override {};
    void on_exit() noexcept {};
};
