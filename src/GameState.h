#pragma once

#include "GameStateDesc.h"
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
};