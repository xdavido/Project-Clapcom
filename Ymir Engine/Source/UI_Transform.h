#ifndef __UI_TRANSFORM_H__
#define __UI_TRANSFORM_H__

#pragma once

#include "Globals.h"
#include "Component.h"
#include "C_UI.h"

#include "../Source/External/MathGeoLib/include/MathGeoLib.h"

#include "../Source/External/ImGui/imgui.h"
#include "../Source/External/ImGui/backends/imgui_impl_sdl2.h"
#include "../Source/External/ImGui/backends/imgui_impl_opengl3.h"
#include "../Source/External/ImGui/misc/cpp/imgui_stdlib.h"
#include "../Source/External/ImGui/imgui_internal.h"

#include "Mesh.h"

#include "Color.h"
#include <string>
#include <vector>

class UI_Transform : public Component
{
public:
	UI_Transform(C_UI* owner);
	~UI_Transform();

	virtual update_status Update(float dt);

	virtual void OnInspector() {};

public:

	// Transform info
	float posX, posY, width, height;

	UI_Bounds* boundsEditorReference;
	UI_Bounds* boundsGameReference;

};
#endif // __UI_TRANSFORM_H__