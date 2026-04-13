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

    void push_back(GameCommand game_command) {
        command_queue.push_back(game_command);
    }

	void process_commands(GameWorld& game_world, WorldRenderer& world_renderer, GameState& game_state) {
        while (!command_queue.empty()) {
            GameCommand current_command = command_queue.front();

            std::cout << "CommandQueue::process_commands - check command" << std::endl;

            if (current_command.command_type == CommandType::SELECT_UNIT) {
                std::cout << "CommandQueue::process_commands - CommandType::SELECT_UNIT" << std::endl;

                game_state.select_unit(current_command.target_unit);
            }

            if (current_command.command_type == CommandType::DESELECT_UNIT) {
                std::cout << "CommandQueue::process_commands - CommandType::DESELECT_UNIT" << std::endl;

                game_state.deselect_unit(current_command.target_unit);
            }

            if (current_command.command_type == CommandType::ATTACK) {
                std::cout << "CommandQueue::process_commands - CommandType::ATTACK" << std::endl;
                current_command.target_unit->attack_at(current_command.target_cell_ij, world_renderer);

                game_world.unpause();
            }

            if (current_command.command_type == CommandType::ACTIVATE_MAIN_WEAPON) {
                std::cout << "CommandQueue::process_commands - CommandType::ACTIVATE_WEAPON" << std::endl;
                current_command.target_unit->activate_main_weapon();

            }

            if (current_command.command_type == CommandType::ACTIVATE_AUX_WEAPON) {
                std::cout << "CommandQueue::process_commands - CommandType::ACTIVATE_AUX_WEAPON" << std::endl;
                current_command.target_unit->activate_aux_weapon();

            }

            if (current_command.command_type == CommandType::MOVE) {
                std::cout << "CommandQueue::process_commands - CommandType::MOVE" << std::endl;

                std::vector<IJ> path = game_world.terrain.find_path(current_command.target_unit->get_home_ij(), current_command.target_cell_ij);

                current_command.target_unit->set_waypoints(path);
                current_command.target_unit->start_waypoints_following(world_renderer);

                game_world.unpause();
            }

            command_queue.pop_front();
        }
	}
private:
};