#pragma once
#include <deque>
#include "GameCommand.h"
#include "StateMachine.h"
#include "UI_InputEvent.h"
#include "GameWorld.h"
#include "GameState.h"

class CommandQueue {
public:

    std::deque<GameCommand> command_queue;

    void push_back(GameCommand game_command);

    void process_commands(GameWorld& game_world, WorldRenderer& world_renderer, GameState& game_state);

private:
    void process_attack(GameCommand& current_command, GameWorld& game_world, WorldRenderer& world_renderer, GameState& game_state);

    void process_move(GameCommand& current_command, GameWorld& game_world, WorldRenderer& world_renderer, GameState& game_state);

    void process_next_turn(GameCommand& current_command, GameWorld& game_world, WorldRenderer& world_renderer, GameState& game_state);

};