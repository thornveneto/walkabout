#pragma once

enum class GameModeEnum {
	MAIN_MENU,
	COMBAT
};

class GameMode {
public:
	virtual ~GameMode() = default;
	virtual void read_input_release_command()=0;
	virtual void process_commands()=0;
	virtual void update()=0;
	virtual void draw()=0;
	virtual void final_loop_actions()=0;
};