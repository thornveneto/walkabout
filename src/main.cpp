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
#include "CommandQueue.h"

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
        //std::deque<GameCommand> command_queue;
        CommandQueue command_queue;

        HUD hud{ command_queue.command_queue };

        auto window = sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "Walkabout");
        window.setFramerateLimit(100);//TODO: this is very basic option - original value 144

        WorldRenderer world_renderer{ window };

        GameWorld game_world{ world_renderer };
        game_world.init();

        //StateMachine<UIState, UI_InputEvent> command_state_machine(std::make_unique<DummyCommandState>(nullptr/*TODO: super bad*/, game_world, world_renderer, command_queue.command_queue));

        //TODO: HOW COME THIS WORKS??? PROBABLY BECAUSE WE DONT CALL IT AND CAN REMOVE IT
        std::cout << "DONT FORGET TO FIX THIS LINE AND DELETE DUMMY COMMAND STATE" << std::endl;

        StateMachine<UIState, UI_InputEvent> command_state_machine(std::make_unique<UnitSelectionState>(nullptr/*TODO: super bad*/, game_world, world_renderer, nullptr, command_queue.command_queue));

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
            command_queue.process_commands(command_state_machine, game_world, world_renderer);


            //---STAGE: UPDATING - temporarily within the state
            hud.set_active_unit(command_state_machine.get_current_state()->active_unit());
            game_world.update();

            //---STAGE: DRAWING
            // Pre drawing
            window.clear();

            //Draw here
            game_world.draw();
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
