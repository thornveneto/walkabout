#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include "Vector2D.h"
#include "WorldRenderer.h"
#include "MovingEntity.h"
#include <map>
#include "GameWorld.h"
#include "Explosion.h"
#include "Unit.h"
#include "CellCube.h"

#include "State.h"
#include "StateMachine.h"
#include "UIState.h"
#include "DummyCommandState.h"
#include "UnitSelectionState.h"
#include "HUD.h"

//main TODO: 
// 1 - DONE: make agents move from tile to tile
// 1.5 DONE: make movement and placement using pre projections
// 2 - DONE: add creating and flying projectile
// 3 - DONE: add projectile hitting an object 
// 3 - DONE: add a few obstacles and basic path-finding. BFS is good
// 4 - DONE: add giving objects a command
// 5 - add image of a person when select it
// 6 - DONE: add capability to kill creatures

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
        HUD hud{};

        auto window = sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "Walkabout");
        window.setFramerateLimit(100);//TODO: this is very basic option - original value 144

        WorldRenderer world_renderer{ window };

        GameWorld game_world;
        game_world.init(world_renderer);

        StateMachine<UIState, UI_InputEvent> command_state_machine(std::make_unique<DummyCommandState>(nullptr/*TODO: super bad*/, game_world, world_renderer));
        command_state_machine.switch_state(std::make_unique<UnitSelectionState>(&command_state_machine, game_world, world_renderer, nullptr));

        /*
        game_world.entity_map.at(1)->set_waypoints({ {0,0},{0,9}, {9,9}, {9,0} });
        game_world.entity_map.at(1)->start_waypoints_following(world_renderer);
        */

        //GAME LOOP

        while (window.isOpen())
        {


            //---STAGE: READING INPUT
            UI_InputEvent ui_input_event = create_ui_event_from_input(window);

            if (ui_input_event.close_window_request) {
                window.close();
            }

            //---STAGE: UPDATING - temporarily within the state

            
            //2
            //---STAGE: DRAWING
            // Pre drawing
            window.clear();

            //Draw here
            game_world.draw(world_renderer);
            command_state_machine.process_event(ui_input_event);

            //TODO: Here's where the tricky bad game bit lies

            // HUD
            hud.set_data(command_state_machine.get_current_state()->active_unit());
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
