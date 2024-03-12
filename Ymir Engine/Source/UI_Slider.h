#pragma once
#ifndef __UI_Slider_H__
#define __UI_Slider_H__
#pragma once

#include "Globals.h"
#include "G_UI.h"
#include "C_UI.h"

#include "UI_Image.h"

enum class SLIDER_DIRECTION
{
	LEFT_TO_RIGHT,
	RIGHT_TO_LEFT,
	BOTTOM_TO_UP,
	UP_TO_RIGHT,

	NONE
};

union uValue
{
	int iValue;
	float fValue;
};

class UI_Slider : public C_UI
{
public:
	// x = 0, y = 0, w = 300, y = 50
	UI_Slider(GameObject* g, int x = 0, int y = 0, G_UI* fill = nullptr, G_UI* handle = nullptr, int w = 300, int h = 50);
	~UI_Slider();

	void OnInspector();
	void OnReferenceDestroyed(void* ptr = nullptr);

	//
	void OnNormal();
	void OnFocused();
	void OnPressed();
	void OnSelected();
	void OnRelease();

public:
	bool isInteractable;

	G_UI* fillImage;
	G_UI* handleImage;

	uValue minValue, maxValue;
	bool useFloat;
	
	uValue value;

	//color
	Color focusedColor;
	Color pressedColor;
	Color selectedColor;
	Color disabledColor;

private:
	SLIDER_DIRECTION direction;
};
#endif // __UI_Slider_H__