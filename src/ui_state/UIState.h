#pragma once
#include "../State.h"
#include "../UI_InputEvent.h"


class WorldRenderer;
class GameWorld;
class Unit;

template <typename T, typename E>
class StateMachine;

class UIState : public State<UI_InputEvent> {
    //Super ugly, but i don't want to create a new state machine
    //Although it might work with templates
protected://TODO: protected or accessors?
    StateMachine<UIState, UI_InputEvent>* state_machine;
    WorldRenderer& world_renderer;
    GameWorld& game_world;
    Unit* _unit{ nullptr };
public:
    UIState(StateMachine<UIState, UI_InputEvent>* state_machine, GameWorld& game_world, WorldRenderer& world_renderer, Unit* unit);

    const Unit* active_unit() const;
};