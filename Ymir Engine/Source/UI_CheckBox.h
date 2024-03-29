#pragma once
#ifndef __UI_CHECKBOX_H__
#define __UI_CHECKBOX_H__
#pragma once

#include "Globals.h"
#include "C_UI.h"

#include "UI_Image.h"
#include "UI_Text.h"

class G_UI;

class UI_Checkbox : public C_UI
{
public:
	// x = 0, y = 0, w = 170, y = 20
	UI_Checkbox(GameObject* g, int x = 0, int y = 0, int w = 170, int h = 170);
	~UI_Checkbox();

	void OnInspector();
	void Draw(bool game) override {};

	void OnNormal();
	void OnFocused();
	void OnPressed();
	void OnSelected();
	void OnRelease();

	void CheckDraggeable();

public:
	bool isInteractable;
	bool isChecked;

	G_UI* bgImg;
	G_UI* cmImg;

	G_UI* displayText;

	//color
	Color focusedColor;
	Color pressedColor;
	Color selectedColor;
	Color disabledColor;
};
#endif // __UI_CHECKBOX_H__