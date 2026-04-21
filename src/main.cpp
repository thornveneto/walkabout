#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <map>
#include <deque>
#include "assert.h"
#include "CombatMode.h"

int main()
{
    try {

        sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({ 1920u, 1080u }), "Walkabout");
        window.setFramerateLimit(100);//TODO: this is very basic option - original value 144

        ResourceManager resource_manager;

        CombatMode combat_mode = CombatMode(window, resource_manager);

        //GAME LOOP
        while (window.isOpen())
        {
            combat_mode.read_input_release_command();
           
            combat_mode.process_commands();

            combat_mode.update();

            //---STAGE: DRAWING
            // Pre drawing
            window.clear();

            ////Draw here
            combat_mode.draw();

            //Call after drawn stuff
            window.display();

            //game_world.sweep_pending_elements();
            combat_mode.final_loop_actions();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Caught a standard exception: " << e.what() << std::endl;
    }
}
