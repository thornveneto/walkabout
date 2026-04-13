#pragma once

#include "entities/Unit.h"
#include "math/IJ.h"

enum class ControlMode {
	UnitSelection,
	UnitSelected,
	CommandExecution
};

struct GameStateDesc {
	bool is_paused{ true };
	Unit* active_unit{ nullptr };
	ControlMode control_mode{ ControlMode::UnitSelection };
	//TODO?? IJ hover_tile;
};