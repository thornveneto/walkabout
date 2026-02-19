#include "UIState.h"
UIState::UIState(StateMachine<UIState, UI_InputEvent>* state_machine, GameWorld& game_world, WorldRenderer& world_renderer, Unit* unit) :
    state_machine{ state_machine },
    game_world{ game_world },
    world_renderer{ world_renderer },
    _unit{ unit }
{
}


const Unit* UIState::active_unit() const {
    return _unit;
}