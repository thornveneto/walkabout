#pragma once

#include "GameStateDesc.h"
#include "Team.h"

/*
	Ensures consitency of the GameStateDesc
*/
class GameState {
public:
	GameStateDesc game_state_desc;

	void select_unit(Unit* unit) {
		game_state_desc.active_unit = unit;
		game_state_desc.control_mode = ControlMode::UnitSelected;

		unit->select();
	}

	void deselect_unit(Unit* unit) {
		game_state_desc.active_unit = nullptr;
		game_state_desc.control_mode = ControlMode::UnitSelection;

		unit->deselect();
	}

	void activate_team(Team* team) {
		if (game_state_desc.active_unit) {
			game_state_desc.active_unit->deselect();
		}

		game_state_desc.active_unit = nullptr;
		game_state_desc.control_mode = ControlMode::UnitSelection;
		game_state_desc.active_team = team;
		game_state_desc.team_ap_remaining = 250;//TODO: hardcoded hacko

	}
};