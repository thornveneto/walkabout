#pragma once
#include <SFML/Graphics.hpp>
#include "State.h"
#include "StateMachine.h"
#include "WorldRenderer.h"
#include "GameWorld.h"
#include "UI_InputEvent.h"
class UIState : public State {
    //Super ugly, but i don't want to create a new state machine
    //Although it might work with templates
protected://TODO: protected or accessors?
    StateMachine<UIState>* state_machine;
    WorldRenderer& world_renderer;
    GameWorld& game_world;
    sf::RenderWindow& window;

    UI_InputEvent _ui_input_event;
public:
    UIState(StateMachine<UIState>* state_machine, GameWorld& game_world, WorldRenderer& world_renderer, sf::RenderWindow& window) : 
        state_machine{ state_machine },
        game_world{ game_world },
        world_renderer{ world_renderer }, window{ window } {

    }

    void set_ui_params(const UI_InputEvent ui_input_event);
};