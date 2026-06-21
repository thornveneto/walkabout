#pragma once

#include "GameMode.h"
#include "UI_InputEvent.h"
#include "ui_utils.h"
#include "ResourceManager.h"
#include "interface_components/Button.h"

class MainMenuMode : public GameMode {
public:
    MainMenuMode(sf::RenderWindow& window, ResourceManager& resource_manager,GameModeEnum& requested_game_mode) :
        window{ window }, resource_manager{ resource_manager },
        btn_new_game("new game", resource_manager.main_font(), { /*m_screen_area.x +*/ 700, /*m_screen_area.y + */400, 100, 50}),
        requested_game_mode{ requested_game_mode }
    {

    }

    void read_input_release_command() {
        //---STAGE: READING INPUT

        UI_InputEvent ui_input_event = create_ui_event_from_input(window);

        if (ui_input_event.close_window_request) {
            window.close();
        }

        if (btn_new_game.fires_on_event(ui_input_event)) {
            std::cout << "MAIN MENU FIRES" << std::endl;
            switch_state = true;
        }
        //OLD - hud.handle_event(ui_input_event, game_state.game_state_desc);
        //OLD - play_field.handle_event(ui_input_event, game_state.game_state_desc);

        //At this staget command will be released
    }

    void process_commands() {
        //---STAGE: APPLYING INPUT
        //here command is actually actionend
        //will unpause the game
        //OLD - command_queue.process_commands(game_world, world_renderer, game_state);
        //TODO: we simplify it for the time being no command queue as in CombatMode, 
        // just a simple state but use the same function for similar pattern
        if (switch_state) {
            requested_game_mode = GameModeEnum::COMBAT;
        }
    }

    void update() {
        //---STAGE: UPDATING - temporarily within the state
        //hud.set_active_unit(command_state_machine.get_current_state()->active_unit());
        //will automatically pause the game when no updates
        
        //OLD - game_world.update();
    }

    void draw() {
        //Draw here
        //OLD - play_field.draw(window, game_state.game_state_desc);
        //OLD - hud.draw(window, game_state.game_state_desc);
        //OLD - cursor.draw(world_renderer, window, game_state.game_state_desc);
        btn_new_game.draw(window);
    }

    void final_loop_actions() {}
private:
    sf::RenderWindow& window;
    ResourceManager& resource_manager;

    //specific
    Button btn_new_game;
    bool switch_state{ false };

    GameModeEnum& requested_game_mode;//TODO: change to proper state management
};