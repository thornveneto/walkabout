#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include "WorldRenderer.h"
#include <map>
#include "GameWorld.h"
#include "State.h"
#include "StateMachine.h"
#include "ui_state/UIState.h"
#include "ui_state/DummyCommandState.h"
#include "ui_state/UnitSelectionState.h"
#include "interface_components/HUD.h"
#include <deque>
#include "GameCommand.h"
#include "ui_state/UnitSelectedState.h"
#include "ui_state/UnitCommandExecutionState.h"

UI_InputEvent create_ui_event_from_input(sf::RenderWindow& window) {
    //---STAGE: READING INPUT
    UI_InputEvent ui_input_event;

    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            ui_input_event.close_window_request = true;
        }
        if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                ui_input_event.left_key_pressed = true;
            }
            if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                ui_input_event.right_key_pressed = true;
            }
        }
    }

    // MOUSE
    sf::Vector2i mouse_local_position = sf::Mouse::getPosition(window); // window is a sf::Window
    ui_input_event.mouse_position = { mouse_local_position.x, mouse_local_position.y };

    return ui_input_event;
}

int main()
{
    try {
        std::deque<GameCommand> command_queue;

        HUD hud{ command_queue };

        auto window = sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "Walkabout");
        window.setFramerateLimit(100);//TODO: this is very basic option - original value 144

        WorldRenderer world_renderer{ window };

        GameWorld game_world;
        game_world.init(world_renderer);

        StateMachine<UIState, UI_InputEvent> command_state_machine(std::make_unique<DummyCommandState>(nullptr/*TODO: super bad*/, game_world, world_renderer, command_queue));
        command_state_machine.switch_state(std::make_unique<UnitSelectionState>(&command_state_machine, game_world, world_renderer, nullptr, command_queue));



        //GAME LOOP
        while (window.isOpen())
        {


            //---STAGE: READING INPUT
            UI_InputEvent ui_input_event = create_ui_event_from_input(window);

            if (ui_input_event.close_window_request) {
                window.close();
            }
            hud.handle_event(ui_input_event);
            command_state_machine.process_event(ui_input_event);//TODO: temporary placement to test as state request must happen in the next frame
           
            //---STAGE: APPLYING INPUT
            while (!command_queue.empty()) {
                GameCommand current_command = command_queue.front();

                if (current_command.command_type == CommandType::SELECT_UNIT) {
                    current_command.target_unit->select();
                    command_state_machine.switch_state(
                        std::make_unique<UnitSelectedState>(&command_state_machine, game_world, world_renderer, current_command.target_unit, command_queue)
                    );
                }

                if (current_command.command_type == CommandType::DESELECT_UNIT) {
                    current_command.target_unit->deselect();

                    command_state_machine.switch_state(
                        std::make_unique<UnitSelectionState>(&command_state_machine, game_world, world_renderer, nullptr, command_queue)
                    );
                }

                if (current_command.command_type == CommandType::ATTACK) {
                    current_command.target_unit->shoot_at(current_command.target_cell_ij, world_renderer);

                    command_state_machine.switch_state(
                        std::make_unique<UnitCommandExecutionState>(&command_state_machine, game_world, world_renderer, current_command.target_unit, command_queue)
                    );
                }

                if (current_command.command_type == CommandType::MOVE) {
                    std::vector<IJ> path = game_world.terrain.find_path(current_command.target_unit->get_home_ij(), current_command.target_cell_ij);

                    current_command.target_unit->set_waypoints(path);
                    current_command.target_unit->start_waypoints_following(world_renderer);

                    command_state_machine.switch_state(
                        std::make_unique<UnitCommandExecutionState>(&command_state_machine, game_world, world_renderer, command_queue.front().target_unit, command_queue)
                    );
                }

                command_queue.pop_front();
            }


            //---STAGE: UPDATING - temporarily within the state
            hud.set_data(command_state_machine.get_current_state()->active_unit());
            game_world.update(world_renderer);

            //---STAGE: DRAWING
            // Pre drawing
            window.clear();

            //Draw here
            game_world.draw(world_renderer);
            command_state_machine.get_current_state()->draw(window);
            hud.draw(window);

            //Call after drawn stuff
            window.display();

            game_world.sweep_pending_elements();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Caught a standard exception: " << e.what() << std::endl;
    }
}
