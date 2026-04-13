#pragma once
#include "entities/Unit.h"

enum class CommandType {
	SELECT_UNIT,
	DESELECT_UNIT,
	MOVE,
	ATTACK,
	ACTIVATE_MAIN_WEAPON,
	ACTIVATE_AUX_WEAPON,
	DE_ACTIVATE_MAIN_WEAPON,
	DE_ACTIVATE_AUX_WEAPON,
	NEXT_TURN
};

struct GameCommand {
public:
	const CommandType command_type;
	const IJ target_cell_ij;
	Unit* const target_unit;

	GameCommand(CommandType command_type, IJ target_cell_ij, Unit* target_unit) :
		command_type{ command_type }, 
		target_cell_ij{ target_cell_ij }, 
		target_unit { target_unit } 
	{

	}
};