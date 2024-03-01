#pragma once

#include "Globals.h"
#include "Application.h"

#include"GameObject.h"
#include"ResourceMesh.h"

#include"CMesh.h"
#include"CScript.h"
#include"CTransform.h"

#include"ModuleInput.h"
#include"ModuleScene.h"
#include"ModuleResourceManager.h"

#include"GameObject.h"
#include"MathGeoLib/include/Math/float3.h"



int GetGamepadButton(MonoObject* x)
{
	if (External != nullptr)
		return External->input->GetGamePadKey(*(int*)mono_object_unbox(x));

	return 0;
}

int GetLeftAxisY()
{
	return External->input->GetGamepadLeftJoystickPositionValueY();
}
int GetLeftAxisX()
{
	return External->input->GetGamepadLeftJoystickPositionValueX();
}
int GetRightAxisY()
{
	return External->input->GetGamepadRightJoystickPositionValueY();
}
int GetRightAxisX()
{
	return External->input->GetGamepadRightJoystickPositionValueX();
}

int GetGamepadLeftTrigger()
{
	if (External != nullptr)
		return External->input->GetLeftTrigger();

	return 0;
}

int GetGamepadRightTrigger()
{
	if (External != nullptr)
		return External->input->GetRightTrigger();

	return 0;
}

void PlayHaptic(float strength, int length) {
	if (External != nullptr) {
		External->input->PlayHaptic(strength, length);
	}
}

int CS_GetControllerType()
{
	if (External != nullptr)
		return External->input->GetControllerType();

	return 0;
}