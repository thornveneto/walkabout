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
#include "Warrior.h"
#include "CellCube.h"

#include "State.h"
#include "StateMachine.h"
#include "UIState.h"
#include "DummyCommandState.h"
#include "UnitCommandSelectionState.h"

//main TODO: 
// 1 - DONE: make agents move from tile to tile
// 1.5 DONE: make movement and placement using pre projections
// 2 - add creating and flying projectile
// 3 - add projectile hitting an object 
// 3 - add a few obstacles and basic path-finding. BFS is good
// 4 - add giving objects a command

int main()
{
    try {
        auto window = sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "Walkabout");
        window.setFramerateLimit(100);//TODO: this is very basic option - original value 144

        WorldRenderer world_renderer{ window };

        GameWorld game_world;
        game_world.init(world_renderer);

        StateMachine<UIState> command_state_machine(std::make_unique<DummyCommandState>(nullptr/*TODO: super bad*/, game_world, world_renderer, window));
        command_state_machine.switch_state(std::make_unique<UnitCommandSelectionState>(&command_state_machine, game_world, world_renderer, window));

        /*
        game_world.entity_map.at(1)->set_waypoints({ {0,0},{0,9}, {9,9}, {9,0} });
        game_world.entity_map.at(1)->start_waypoints_following(world_renderer);

        //temporary warrior to spawn projectiles
        Warrior* warrior = dynamic_cast<Warrior*>(game_world.entity_map.at(2).get());
        if (warrior) {
            warrior->shoot_at(2, 10, world_renderer);
        }
        */

        // UI 
        // Define cube vertices directly in 2D (centered around origin)
        //X - CellCube ui_cell_selection(world_renderer.hw, world_renderer.hh, world_renderer.cell_height, sf::Color::Cyan);

        /*
        CellCube todo_marker(world_renderer.hw, world_renderer.hh, world_renderer.cell_height, sf::Color::Magenta);
        */

        //GAME LOOP
        game_world.toggle_pause(); //to make things running initially

        while (window.isOpen())
        {


            //---STAGE: READING INPUT
            UI_InputEvent ui_input_event;

            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                {
                    window.close();
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

            command_state_machine.get_current_state()->set_ui_params(ui_input_event);

            //X - auto mouse_ij = world_renderer.screen_tile(mouse_local_position.x, mouse_local_position.y);
            //---STAGE: UPDATING

            
            //2
            //---STAGE: DRAWING
            // Pre drawing
            window.clear();

            //Draw here
            game_world.draw(world_renderer);
            command_state_machine.execute();

            //X - ui_cell_selection.draw(window, 
            //    sf::Vector2f{ 
            //        world_renderer.tile_world_x(mouse_ij.first, mouse_ij.second),
            //        world_renderer.tile_world_y(mouse_ij.first, mouse_ij.second)
            //    }
            //);

            /*
            std::pair<int, int> marker_tile = game_world.entity_map.at(1)->get_home_ij();

            todo_marker.draw(window,
                sf::Vector2f{
                    world_renderer.tile_world_x(marker_tile.first, marker_tile.second),
                    world_renderer.tile_world_y(marker_tile.first, marker_tile.second)
                }
            );
            */

            //Call after drawn stuff
            window.display();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Caught a standard exception: " << e.what() << std::endl;
    }
}
