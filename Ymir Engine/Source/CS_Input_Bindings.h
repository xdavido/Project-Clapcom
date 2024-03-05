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



//int GetGamepadButton(MonoObject* x)
//{ 
//	if (External != nullptr)
//		return External->input->GetGamePadKey(*(int*)mono_object_unbox(x));
//
//	return 0;
//}

float GetLeftAxisY()
{
	return External->input->GetGamepadLeftJoystickPositionValueY();
}
float GetLeftAxisX()
{
	return External->input->GetGamepadLeftJoystickPositionValueX();
}
float GetRightAxisY()
{
	return External->input->GetGamepadRightJoystickPositionValueY();
}
float GetRightAxisX()
{
	return External->input->GetGamepadRightJoystickPositionValueX();
}

float GetGamepadLeftTrigger()
{
	if (External != nullptr)
		return External->input->GetGamepadLeftTriggerValue();

	return 0;
}

float GetGamepadRightTrigger()
{
	if (External != nullptr)
		return External->input->GetGamepadRightTriggerValue();

	return 0;
}

//void PlayHaptic(float strength, int length) {
//	if (External != nullptr) {
//		External->input->PlayHaptic(strength, length);
//	}
//}
//
//int CS_GetControllerType()
//{
//	if (External != nullptr)
//		return External->input->GetControllerType();
//
//	return 0;
//}