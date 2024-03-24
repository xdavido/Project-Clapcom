#pragma once
#ifndef __UI_INPUTBOX_H__
#define __UI_INPUTBOX_H__
#pragma once

#include "Globals.h"
#include "C_UI.h"
#include "UI_Text.h"
#include "UI_Image.h"

class UI_InputBox : public C_UI
{
public:
	// x = 0, y = 0, w = 300, y = 50
	UI_InputBox(GameObject* g, float x = 0, float y = 0, float w = 300, float h = 50);
	~UI_InputBox();

	update_status Update(float dt) override;

	void OnInspector();

	void OnNormal();
	void OnFocused();
	void OnPressed();
	void OnSelected();
	void OnRelease();

public:
	std::string text;
	UI_Image* image;

	int maxChars;
	UI_Text* displayText;

	//color
	Color focusedColor;
	Color pressedColor;
	Color selectedColor;
	Color disabledColor;

private:
	float fontSize;
	bool isWriting;
};
#endif // __UI_INPUTBOX_H__