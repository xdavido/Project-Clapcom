#pragma once

#include "Module.h"
#include "Globals.h"

#include "External/SDL/include/SDL_gamecontroller.h"

#define MAX_MOUSE_BUTTONS 5

#define SDL_MAX_SINT16 32767
#define MAX_CONTROLLERS 2

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

// Gamepad Management

struct GameController {

	float j1_x, j1_y, j2_x, j2_y, LT, RT;
	KEY_STATE buttons[SDL_CONTROLLER_BUTTON_MAX];

};

enum class GamepadJoystick { LEFT, RIGHT };
enum class GamepadJoystickAxis { X, Y };
enum class GamepadJoystickDirection { POSITIVE, NEGATIVE };

class ModuleInput : public Module
{
public:
	
	ModuleInput(Application* app, bool start_enabled = true);
	virtual ~ModuleInput();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	bool CleanUp() override;

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

	// ------------ Gamepad Management ------------

	bool IsGamepadON();

	// Buttons and Joysticks Mapping

	bool IsGamepadButtonPressed(SDL_GameControllerButton button, KEY_STATE state);
	bool IsGamepadJoystickDirection(GamepadJoystick joystick, GamepadJoystickAxis axis, GamepadJoystickDirection direction);

	// Idle Management

	bool AreGamepadButtonsIdle();
	bool IsGamepadJoystickIdle(GamepadJoystick joystick);
	bool IsGamepadIdle();

public:

	// Quit

	bool quit = false;

	// Drag & Drop

	const char* droppedFileDirectory;
	bool droppedFile = false;

	// Gamepad Management

	SDL_GameController* sdl_controllers[MAX_CONTROLLERS];
	GameController controllers[MAX_CONTROLLERS];

	std::vector<GameController*> activeControllers;
	int num_controllers;

	bool gamepadON = false;

private:

	KEY_STATE* keyboard;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	int mouse_x;
	int mouse_y;
	int mouse_z;
	int mouse_x_motion;
	int mouse_y_motion;
	//int mouse_z_motion;

	// Gamepad Deadzone Management
	float ReduceJoystickValue(bool controllerON, float v1, float min, float clamp_to);

};