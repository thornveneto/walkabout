#include "DummyCommandState.h"
#include "StateMachine.h"
#include "WorldRenderer.h"
#include "GameWorld.h"

DummyCommandState::DummyCommandState(StateMachine<UIState, UI_InputEvent>* state_machine, GameWorld& game_world, WorldRenderer& world_renderer)
    : UIState(state_machine, game_world, world_renderer) {
}