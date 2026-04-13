#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include "WorldRenderer.h"
#include <map>
#include "GameWorld.h"
#include "State.h"
#include "StateMachine.h"
#include "interface_components/HUD.h"
#include <deque>
#include "GameCommand.h"
#include "CommandQueue.h"
#include "ResourceManager.h"
#include "interface_components/PlayField.h"
#include "GameState.h"

#include "assert.h"
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
        if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
                ui_input_event.left_key_released = true;
            }
            if (mouseButtonReleased->button == sf::Mouse::Button::Right) {
                ui_input_event.right_key_released = true;
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

        ResourceManager resource_manager;

        HUD hud{ resource_manager.main_font(), command_queue.command_queue };

        auto window = sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "Walkabout");
        window.setFramerateLimit(100);//TODO: this is very basic option - original value 144

        WorldRenderer world_renderer{ window };

        GameState game_state;

        GameWorld game_world{ world_renderer };

        PlayField play_field{ game_world, world_renderer, command_queue.command_queue };

        Cursor cursor{ game_world, world_renderer.hw, world_renderer.hh, world_renderer.cell_height };

        game_world.init();

        //StateMachine<UIState, UI_InputEvent> command_state_machine(std::make_unique<DummyCommandState>(nullptr/*TODO: super bad*/, game_world, world_renderer, command_queue.command_queue));

        //TODO: HOW COME THIS WORKS??? PROBABLY BECAUSE WE DONT CALL IT AND CAN REMOVE IT
        std::cout << "DONT FORGET TO FIX THIS LINE AND DELETE DUMMY COMMAND STATE" << std::endl;        

        game_state.activate_team(game_world.get_team(1));

        //GAME LOOP
        while (window.isOpen())
        {


            //---STAGE: READING INPUT
            UI_InputEvent ui_input_event = create_ui_event_from_input(window);

            if (ui_input_event.close_window_request) {
                window.close();
            }
            hud.handle_event(ui_input_event, game_state.game_state_desc);
            play_field.handle_event(ui_input_event, game_state.game_state_desc);
            //At this staget command will be released
           
            //---STAGE: APPLYING INPUT
            //here command is actually actionend
            //will unpause the game
            command_queue.process_commands(game_world, world_renderer, game_state);


            //---STAGE: UPDATING - temporarily within the state
            //hud.set_active_unit(command_state_machine.get_current_state()->active_unit());
            //will automatically pause the game when no updates
            game_world.update();

            //---STAGE: DRAWING
            // Pre drawing
            window.clear();

            //Draw here
            play_field.draw(window, game_state.game_state_desc);
            hud.draw(window, game_state.game_state_desc);
            cursor.draw(world_renderer, window, game_state.game_state_desc);

            //Call after drawn stuff
            window.display();

            game_world.sweep_pending_elements();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Caught a standard exception: " << e.what() << std::endl;
    }
}
