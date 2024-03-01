#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleResourceManager.h"
#include "Log.h"
#include "CCamera.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"

#include "External/Optick/include/optick.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0)
	{
		LOG("SDL_GAMECONTROLLER could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("[ERROR] SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	num_controllers = SDL_NumJoysticks();

	for (int i = 0; i < num_controllers; i++) {

		if (SDL_IsGameController(i)) {

			sdl_controllers[i] = SDL_GameControllerOpen(i);

			activeControllers.push_back((GameController*)sdl_controllers[i]);

		}

	}

	return ret;
}

bool ModuleInput::Start()
{
	return true;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	OPTICK_EVENT();

	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for (int i = 0; i < 5; ++i)
	{
		if (buttons & SDL_BUTTON(i))
		{
			if (mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if (mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

		switch (e.type)
		{
		case SDL_MOUSEWHEEL:
		{
			mouse_z = e.wheel.y;
			break;
		}
		case SDL_MOUSEMOTION:
		{
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;

			break;
		}
		case SDL_QUIT:
		{
			return UPDATE_STOP;
			break;
		}
		case SDL_WINDOWEVENT:
		{
			switch (e.window.event) {

			case SDL_WINDOWEVENT_RESIZED:
			{
#ifndef _STANDALONE

				App->renderer3D->OnResize(e.window.data1, e.window.data2);

#else

				App->scene->gameCameraComponent->framebuffer.loaded = false;

				App->renderer3D->OnResize(e.window.data1, e.window.data2);
				App->window->width = e.window.data1;
				App->window->height = e.window.data2;

				glViewport(0, 0, e.window.data1, e.window.data2);
				App->scene->gameCameraComponent->SetAspectRatio((float)(External->window->width / (float)External->window->height));

#endif // _STANDALONE

				break;
			}

			case SDL_WINDOWEVENT_MAXIMIZED:
			{

#ifndef _STANDALONE

				App->renderer3D->OnResize(e.window.data1, e.window.data2);

#else

				App->scene->gameCameraComponent->framebuffer.loaded = false;

				App->renderer3D->OnResize(e.window.data1, e.window.data2);
				App->scene->gameCameraComponent->SetAspectRatio((float)(External->window->width / (float)External->window->height));

#endif // _STANDALONE	

				break;
			}
			}
			break;
		}
		case SDL_DROPFILE:
		{ // In case if dropped file
			droppedFile = true;
			droppedFileDirectory = e.drop.file;

			App->resourceManager->ImportFileToEngine(droppedFileDirectory);

			// Shows directory of dropped file
			SDL_free(&droppedFileDirectory); // Free dropped_filedir memory

			break;
		}

		}
	}

	// Gamepad Management

	SDL_GameControllerUpdate();

	for (int i = 0; i < num_controllers; ++i) {

		for (int j = 0; j < SDL_CONTROLLER_BUTTON_MAX; ++j) {

			if (SDL_GameControllerGetButton(sdl_controllers[i], (SDL_GameControllerButton)j)) {

				controllers[i].buttons[j] = (controllers[i].buttons[j] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;

			}
			else {

				controllers[i].buttons[j] = (controllers[i].buttons[j] == KEY_REPEAT || controllers[i].buttons[j] == KEY_DOWN) ? KEY_UP : KEY_IDLE;

			}
				
		}

		controllers[i].j1_x = SDL_GameControllerGetAxis(sdl_controllers[i], SDL_CONTROLLER_AXIS_LEFTX);
		controllers[i].j1_y = SDL_GameControllerGetAxis(sdl_controllers[i], SDL_CONTROLLER_AXIS_LEFTY);
		controllers[i].LT = SDL_GameControllerGetAxis(sdl_controllers[i], SDL_CONTROLLER_AXIS_TRIGGERLEFT);

		controllers[i].j2_x = SDL_GameControllerGetAxis(sdl_controllers[i], SDL_CONTROLLER_AXIS_RIGHTX);
		controllers[i].j2_y = SDL_GameControllerGetAxis(sdl_controllers[i], SDL_CONTROLLER_AXIS_RIGHTY);
		controllers[i].RT = SDL_GameControllerGetAxis(sdl_controllers[i], SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

	}

	if (!activeControllers.empty()) {

		gamepadON = true;

	}
	else {

		gamepadON = false;

	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");

	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);

	return true;
}

bool ModuleInput::AreGamepadButtonsIdle()
{
	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i) {

		if (App->input->controllers[0].buttons[i] != KEY_IDLE) {
			return false;
		}
	}

	return true;
}

bool ModuleInput::IsGamepadJoystickIdle(GamepadJoystick joystick)
{
	switch (joystick)
	{
	case GamepadJoystick::LEFT:
	{
		return App->input->ReduceJoystickValue(SDL_IsGameController(0), App->input->controllers[0].j1_x, 10000, 2) == 0 &&
			App->input->ReduceJoystickValue(SDL_IsGameController(0), App->input->controllers[0].j1_y, 10000, 2) == 0;

		break;
	}
	case GamepadJoystick::RIGHT:
	{
		return App->input->ReduceJoystickValue(SDL_IsGameController(0), App->input->controllers[0].j2_x, 10000, 2) == 0 &&
			App->input->ReduceJoystickValue(SDL_IsGameController(0), App->input->controllers[0].j2_y, 10000, 2) == 0;

		break;
	}

	}
}

bool ModuleInput::IsGamepadIdle()
{
	return AreGamepadButtonsIdle() &&
		IsGamepadJoystickIdle(GamepadJoystick::LEFT) &&
		IsGamepadJoystickIdle(GamepadJoystick::RIGHT);
}

float ModuleInput::ReduceJoystickValue(bool controllerON, float v1, float min, float clamp_to) {

	if (controllerON) {

		float sign = v1 / fabs(v1);

		float reduced = v1 - ((fabs(v1) > min) ? sign * min : v1);

		float to_1 = reduced / (float)(SDL_MAX_SINT16);

		float reclamped = to_1 * clamp_to;

		return reclamped;

	}
	else {

		return 0;
	}

}

float ModuleInput::ReduceTriggerValue(bool controllerON, float triggerValue)
{
	if (controllerON) {

		// Scale triggerValue to the range [0, 100] based on 32767
		triggerValue = (triggerValue / 32767.0f) * 100.0f;

		// Clamp triggerValue to the range [0, 100]
		triggerValue = MAX(0.0f, MIN(100.0f, triggerValue));

		return triggerValue;
	}
	else {

		return 0;

	}

}

bool ModuleInput::IsGamepadON()
{
	return gamepadON;
}

bool ModuleInput::IsGamepadButtonPressed(SDL_GameControllerButton button, KEY_STATE state)
{
	return controllers[0].buttons[button] == state;
}

bool ModuleInput::IsGamepadJoystickDirection(GamepadJoystick joystick, GamepadJoystickAxis axis, GamepadJoystickDirection direction)
{
	switch (joystick)
	{
	case GamepadJoystick::LEFT:
	{
		switch (axis)
		{
		case GamepadJoystickAxis::X:
		{
			switch (direction)
			{
			case GamepadJoystickDirection::POSITIVE:
			{
				// Movement Right
				return ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j1_x, 10000, 2) > 0;
				break;
			}
			case GamepadJoystickDirection::NEGATIVE:
			{
				// Movement Left
				return ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j1_x, 10000, 2) < 0;
				break;
			}
			}
			break;
		}
		case GamepadJoystickAxis::Y:
		{
			switch (direction)
			{
			case GamepadJoystickDirection::POSITIVE:
			{
				// Movement Down
				return ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j1_y, 10000, 2) > 0;
				break;
			}
			case GamepadJoystickDirection::NEGATIVE:
			{
				// Movement Up
				return ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j1_y, 10000, 2) < 0;
				break;
			}
			}
			break;
		}
		}
		break;
	}
	case GamepadJoystick::RIGHT:
	{
		switch (axis)
		{
		case GamepadJoystickAxis::X:
		{
			switch (direction)
			{
			case GamepadJoystickDirection::POSITIVE:
			{
				// Camera Right
				return ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j2_x, 10000, 2) > 0;
				break;
			}
			case GamepadJoystickDirection::NEGATIVE:
			{
				// Camera Left
				return ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j2_x, 10000, 2) < 0;
				break;
			}
			}
			break;
		}
		case GamepadJoystickAxis::Y:
		{
			switch (direction)
			{
			case GamepadJoystickDirection::POSITIVE:
			{
				// Camera Down
				return ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j2_y, 10000, 2) > 0;
				break;
			}
			case GamepadJoystickDirection::NEGATIVE:
			{
				// Camera Up
				return ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j2_y, 10000, 2) < 0;
				break;
			}
			}
			break;
		}
		}
		break;
	}

	}

}

float ModuleInput::GetGamepadLeftTriggerValue()
{
	return ReduceTriggerValue(SDL_IsGameController(0), controllers[0].LT);
}

float ModuleInput::GetGamepadRightTriggerValue()
{
	return ReduceTriggerValue(SDL_IsGameController(0), controllers[0].RT);
}

float ModuleInput::GetGamepadLeftJoystickPositionValueX()
{
	return ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j1_x, 10000, 2);

}

float ModuleInput::GetGamepadLeftJoystickPositionValueY()
{
	return ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j1_y, 10000, 2);
}

float ModuleInput::GetGamepadRightJoystickPositionValueX()
{
	return ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j2_x, 10000, 2);
}

float ModuleInput::GetGamepadRightJoystickPositionValueY()
{
	return ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j2_y, 10000, 2);
}

float ModuleInput::GetGamepadJoystickPositionValueX(GamepadJoystick joystick)
{
	switch (joystick)
	{
	case GamepadJoystick::LEFT:
	{
		return ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j1_x, 10000, 2);

		break;

	}
	case GamepadJoystick::RIGHT:
	{
		return ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j2_x, 10000, 2);
		break;
	}
	}

}

float ModuleInput::GetGamepadJoystickPositionValueY(GamepadJoystick joystick)
{
	switch (joystick)
	{
	case GamepadJoystick::LEFT:
	{
		return ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j1_y, 10000, 2);

		break;

	}
	case GamepadJoystick::RIGHT:
	{
		return ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j2_y, 10000, 2);
		break;
	}
	}

}

float2 ModuleInput::GetGamepadJoystickPositionValues(GamepadJoystick joystick)
{
	switch (joystick)
	{
	case GamepadJoystick::LEFT:
	{
		return float2(ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j1_x, 10000, 2), ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j1_y, 10000, 2));

		break;
	
	}
	case GamepadJoystick::RIGHT:
	{
		return float2(ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j2_x, 10000, 2), ReduceJoystickValue(SDL_IsGameController(0), controllers[0].j2_y, 10000, 2));

		break;
	}
	}

}

// ---------------- New Gamepad Management ----------------

//// Checking if the gamepad is plugged and ON
//if (App->input->IsGamepadON()) {
//
//}
//
//// Checking if the A button is pressed
//if (App->input->IsGamepadButtonPressed(SDL_CONTROLLER_BUTTON_A, KEY_DOWN)) {
//	
//}
//
//// Checking if the left joystick is moved up
//if (App->input->IsGamepadJoystickDirection(GamepadJoystick::LEFT, GamepadJoystickAxis::Y, GamepadJoystickDirection::NEGATIVE)) {
//	
//}
//
//// Checking if the left joystick is moved right
//if (App->input->IsGamepadJoystickDirection(GamepadJoystick::LEFT, GamepadJoystickAxis::X, GamepadJoystickDirection::POSITIVE)) {
//
//}
//
//// Checking if the right joystick is moved left
//if (App->input->IsGamepadJoystickDirection(GamepadJoystick::RIGHT, GamepadJoystickAxis::X, GamepadJoystickDirection::NEGATIVE)) {
//
//}
//
//// Checking if all buttons on the gamepad are idle
//if (App->input->AreGamepadButtonsIdle()) {
//	
//}
//
//// Checking if the right joystick is idle
//if (App->input->IsGamepadJoystickIdle(GamepadJoystick::RIGHT)) {
//	
//}
//
//// Checking if the entire gamepad is idle
//if (App->input->IsGamepadIdle()) {
//	
//}

// ---------------- Old Gamepad Management ----------------

//// Button Management
//if (App->input->controllers[0].buttons[SDL_CONTROLLER_BUTTON_B] == KEY_DOWN) {}
//
//// Idle Management
//if (App->input->ReduceJoystickValue(SDL_IsGameController(0), App->input->controllers[0].j1_x, 10000, 2) == 0 &&
//	App->input->ReduceJoystickValue(SDL_IsGameController(0), App->input->controllers[0].j1_y, 10000, 2) == 0) {}
//
//// Movement Up
//if (App->input->ReduceJoystickValue(SDL_IsGameController(0), App->input->controllers[0].j1_y, 10000, 2) < 0) {}
//
//// Movement Down
//if (App->input->ReduceJoystickValue(SDL_IsGameController(0), App->input->controllers[0].j1_y, 10000, 2) > 0) {}
//
//// Movement Left
//if (App->input->ReduceJoystickValue(SDL_IsGameController(0), App->input->controllers[0].j1_x, 10000, 2) < 0) {}
//
//// Movement Right
//if (App->input->ReduceJoystickValue(SDL_IsGameController(0), App->input->controllers[0].j1_x, 10000, 2) > 0) {}