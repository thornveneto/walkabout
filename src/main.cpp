#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <map>
#include <deque>
#include "assert.h"
#include "CombatMode.h"
#include "MainMenuMode.h"

int main()
{
    try {

        sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "Walkabout");
        window.setFramerateLimit(100);//TODO: this is very basic option - original value 144

        ResourceManager resource_manager;

        
        GameModeEnum requested_game_mode{ GameModeEnum::MAIN_MENU };

        CombatMode combat_mode = CombatMode(window, resource_manager);
        MainMenuMode main_menu_mode = MainMenuMode(window, resource_manager, requested_game_mode);

        GameMode* game_mode{ &main_menu_mode }; //TODO: change to proper state management

        //GAME LOOP
        //?next step add game over bit
        //?on game over either start new game or exit
        while (window.isOpen())
        {
            //TODO: hacko way until proper state management
            if (requested_game_mode == GameModeEnum::MAIN_MENU && game_mode != &main_menu_mode) {
                game_mode = &main_menu_mode;
            }
            else if (requested_game_mode == GameModeEnum::COMBAT && game_mode != &combat_mode) {
                game_mode = &combat_mode;
            }

            game_mode->read_input_release_command();

            game_mode->process_commands();

            game_mode->update();

            //---STAGE: DRAWING
            // Pre drawing
            window.clear();

            //Draw here
            game_mode->draw();

            //Call after drawn stuff
            window.display();

            //post draw actions. TODO: why can't we just put them in update?
            game_mode->final_loop_actions();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Caught a standard exception: " << e.what() << std::endl;
    }
}
