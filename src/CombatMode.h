#pragma once
#include <SFML/Graphics.hpp>
#include "UI_InputEvent.h"
#include "ResourceManager.h"
#include "CommandQueue.h"
#include "interface_components/HUD.h"
#include "interface_components/PlayField.h"
#include "interface_components/Cursor.h"
#include "GameMode.h"
#include "ui_utils.h"

class CombatMode : public GameMode {
public:
    CombatMode(sf::RenderWindow& window, ResourceManager& resource_manager) :
        window{ window }, resource_manager{ resource_manager },
        hud{ resource_manager.main_font(), command_queue.command_queue }, world_renderer{ window },
        game_world{ world_renderer }, play_field{ game_world, world_renderer, command_queue.command_queue },
        cursor{ game_world, world_renderer.hw, world_renderer.hh, world_renderer.cell_height }
    {
        game_world.init();
        game_state.activate_team(game_world.get_team(1));
    }

    void read_input_release_command() {
        //---STAGE: READING INPUT

        UI_InputEvent ui_input_event = create_ui_event_from_input(window);

        if (ui_input_event.close_window_request) {
            window.close();
        }
        hud.handle_event(ui_input_event, game_state.game_state_desc);
        play_field.handle_event(ui_input_event, game_state.game_state_desc);

        //At this staget command will be released
    }

    void process_commands() {
        //---STAGE: APPLYING INPUT
        //here command is actually actionend
        //will unpause the game
        command_queue.process_commands(game_world, world_renderer, game_state);
    }

    void update() {
        //---STAGE: UPDATING - temporarily within the state
        //hud.set_active_unit(command_state_machine.get_current_state()->active_unit());
        //will automatically pause the game when no updates
        game_world.update();
    }

    void draw() {
        //Draw here
        play_field.draw(window, game_state.game_state_desc);
        hud.draw(window, game_state.game_state_desc);
        cursor.draw(world_renderer, window, game_state.game_state_desc);
    }

    void final_loop_actions() {
        game_world.sweep_pending_elements();
    }
private:
    sf::RenderWindow& window;
    ResourceManager& resource_manager;

    //specific
    CommandQueue command_queue;
    HUD hud;
    WorldRenderer world_renderer;
    GameState game_state;
    GameWorld game_world;
    PlayField play_field;
    Cursor cursor;
};