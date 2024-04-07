#ifndef __UI_H__
#define __UI_H__

#pragma once

#include "Globals.h"
#include "Component.h"

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

class UI_Transform;

struct UI_Bounds
{
	GLuint VBO; //vertex buffer object
	GLuint EBO; //element buffer object
	GLuint VAO;
	GLuint id_tex_uvs; //texture UVs

	GLuint textureID;
	AABB local_aabb;//local AABB

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	bool InitBuffers();
	void RegenerateVBO();
	void DeleteBuffers();
};

enum class UI_STATE
{
	DISABLED,
	NORMAL,
	FOCUSED,
	PRESSED,
	RELEASE,
	SELECTED,

	NONE
};

enum class UI_TYPE
{
	CANVAS,
	IMAGE,
	TEXT,
	BUTTON,
	INPUTBOX,
	CHECKBOX,
	SLIDER,

	NONE
};

class C_UI : public Component
{
public:
	C_UI(UI_TYPE ui_t, ComponentType t, GameObject* g, std::string n = "UI", float x = 0, float y = 0, float w = 200, float h = 100, Color c = { 1, 1, 1, 1 });
	~C_UI();

	virtual update_status Update(float dt);

	virtual void OnInspector() {};
	virtual void Draw(bool game);
	virtual void DebugDraw();

	// UI logic
	void StateLogic();
	virtual void OnNormal();
	virtual void OnFocused();
	virtual void OnPressed();
	virtual void OnRelease();
	virtual void OnSelected();
	virtual void OnDisabled();

	void SetState(UI_STATE uiState);
	bool MouseCheck(float2 mouse);

	virtual void UpdateUITransform();

	void DrawABB();
	void DrawOBB();
	void UpdateBoundingBoxes();
	void Drag(float dt);

	void FadeUI(float dt);

public:
	UI_TYPE UI_type;
	UI_STATE state;

	// Transform info
	float posX, posY, width, height;

	//color
	Color color;

	//Mouse pick
	AABB local_aabb;//local AABB
	AABB global_aabb;//global AABB
	OBB obb;//global OB

	bool isDragging;
	bool isDraggeable;
	// x, y, w, h
	ImVec4 dragLimits;

	bool fade;

	UI_Bounds* boundsEditor;
	UI_Bounds* boundsGame;

	float3 scaleBounds;

	UI_Transform* transformUI;
	bool dirty_;

	bool tabNav_;
};
#endif // __UI_H__