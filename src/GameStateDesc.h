#pragma once

#include "entities/Unit.h"
#include "math/IJ.h"
#include "Team.h"

enum class ControlMode {
	UnitSelection,
	UnitSelected,
	CommandExecution
};

struct GameStateDesc {
	bool is_paused{ true };
	Unit* active_unit{ nullptr };
	ControlMode control_mode{ ControlMode::UnitSelection };
	//int team_ap_remaining{ 0 };
	Team* active_team{ nullptr };
	//TODO?? IJ hover_tile;
};