#include "CommandQueue.h"
#include "./entities/Unit.h"
#include "./entities/Weapon.h"

void CommandQueue::push_back(GameCommand game_command) {
    command_queue.push_back(game_command);
}

void CommandQueue::process_commands(GameWorld& game_world, WorldRenderer& world_renderer, GameState& game_state) {
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
            process_attack(current_command, game_world, world_renderer, game_state);
        }

        if (current_command.command_type == CommandType::ACTIVATE_MAIN_WEAPON) {
            std::cout << "CommandQueue::process_commands - CommandType::ACTIVATE_WEAPON" << std::endl;
            current_command.target_unit->activate_main_weapon();

        }

        if (current_command.command_type == CommandType::DE_ACTIVATE_MAIN_WEAPON) {
            std::cout << "CommandQueue::process_commands - CommandType::DE_ACTIVATE_WEAPON" << std::endl;
            current_command.target_unit->de_activate_main_weapon();

        }

        if (current_command.command_type == CommandType::ACTIVATE_AUX_WEAPON) {
            std::cout << "CommandQueue::process_commands - CommandType::ACTIVATE_AUX_WEAPON" << std::endl;
            current_command.target_unit->activate_aux_weapon();

        }

        if (current_command.command_type == CommandType::DE_ACTIVATE_AUX_WEAPON) {
            std::cout << "CommandQueue::process_commands - CommandType::DE_ACTIVATE_AUX_WEAPON" << std::endl;
            current_command.target_unit->de_activate_aux_weapon();

        }

        if (current_command.command_type == CommandType::MOVE) {
            process_move(current_command, game_world, world_renderer, game_state);
        }

        if (current_command.command_type == CommandType::NEXT_TURN) {

            process_next_turn(current_command, game_world, world_renderer, game_state);

        }

        command_queue.pop_front();
    }
}

void CommandQueue::process_attack(GameCommand& current_command, GameWorld& game_world, WorldRenderer& world_renderer, GameState& game_state) {
    std::cout << "CommandQueue::process_commands - CommandType::ATTACK" << std::endl;

    current_command.target_unit->attack_at(current_command.target_cell_ij, world_renderer);

    game_world.unpause();
}

void CommandQueue::process_move(GameCommand& current_command, GameWorld& game_world, WorldRenderer& world_renderer, GameState& game_state) {
    std::cout << "CommandQueue::process_commands - CommandType::MOVE" << std::endl;

    int remaining_action_points = current_command.target_unit->action_points();

    std::vector<IJ> path = game_world.terrain.find_path(current_command.target_unit->get_home_ij(), current_command.target_cell_ij);

    if (path.size() > remaining_action_points + 1) {//because waypoints include the starting point
        path.resize(remaining_action_points + 1); //because waypoints include the starting point
        current_command.target_cell_ij = path[path.size() - 1];
    }

    current_command.target_unit->reduce_action_points(path.size() - 1); //because waypoints include the starting point

    current_command.target_unit->set_waypoints(path);
    current_command.target_unit->start_waypoints_following(world_renderer);

    game_world.unpause();
}

void CommandQueue::process_next_turn(GameCommand& current_command, GameWorld& game_world, WorldRenderer& world_renderer, GameState& game_state) {
    std::cout << "CommandQueue::process_commands - CommandType::NEXT_TURN" << std::endl;

    //TODO: temporary way
    if (game_state.game_state_desc.active_team->team_id() == 1) {
        game_state.activate_team(game_world.get_team(2));
    }
    else {
        game_state.activate_team(game_world.get_team(1));
    }

    game_world.reset_team_action_points(game_state.game_state_desc.active_team->team_id());
}