#pragma once
#include <SFML/Graphics.hpp>
#include "State.h"
#include "StateMachine.h"
#include "WorldRenderer.h"
#include "GameWorld.h"
#include "UI_InputEvent.h"
class UIState : public State<UI_InputEvent> {
    //Super ugly, but i don't want to create a new state machine
    //Although it might work with templates
protected://TODO: protected or accessors?
    StateMachine<UIState, UI_InputEvent>* state_machine;
    WorldRenderer& world_renderer;
    GameWorld& game_world;
    sf::RenderWindow& window;

public:
    UIState(StateMachine<UIState, UI_InputEvent>* state_machine, GameWorld& game_world, WorldRenderer& world_renderer, sf::RenderWindow& window) :
        state_machine{ state_machine },
        game_world{ game_world },
        world_renderer{ world_renderer }, window{ window } {

    }
};