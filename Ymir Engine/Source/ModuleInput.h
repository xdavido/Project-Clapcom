#pragma once
#include "Module.h"
#include "Globals.h"
#include <string>

#define MAX_MOUSE_BUTTONS 5

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	
	ModuleInput(Application* app, bool start_enabled = true);
	virtual ~ModuleInput();

	bool Init() override;
	update_status PreUpdate(float dt) override;
	bool CleanUp() override;

	// --- UI ---
	void HandleInput(SDL_Event event);

	//	
	void SetInputActive(std::string& strToStore, bool getInput = true);
	void SetMaxChars(int limit);


	KEY_STATE GetKey(int id) const
	{
		return keyboard[id];
	}

	KEY_STATE GetMouseButton(int id) const
	{
		return mouse_buttons[id];
	}

	int GetMouseX() const
	{
		return mouse_x;
	}

	int GetMouseY() const
	{
		return mouse_y;
	}

	int GetMouseZ() const
	{
		return mouse_z;
	}

	int GetMouseXMotion() const
	{
		return mouse_x_motion;
	}

	int GetMouseYMotion() const
	{
		return mouse_y_motion;
	}

	bool ModuleInput::GetInputActive() const
	{
		return getInput_B;
	}


private:
	KEY_STATE* keyboard;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	int mouse_x;
	int mouse_y;
	int mouse_z;
	int mouse_x_motion;
	int mouse_y_motion;
	//int mouse_z_motion;

	int maxChars;
	bool getInput_B;

public:

	bool quit = false;

	const char* droppedFileDirectory;
	bool droppedFile = false;

	std::string* strToChange;
	std::string strBU;
};