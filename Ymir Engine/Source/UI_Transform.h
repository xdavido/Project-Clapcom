#ifndef __UI_TRANSFORM_H__
#define __UI_TRANSFORM_H__

#pragma once

#include "C_UI.h"

enum class UI_ANCHOR
{
	TOP_LEFT,
	TOP,
	TOP_RIGHT,
	LEFT,
	CENTER,
	RIGHT,
	BOTTOM_LEFT,
	BOTTOM,
	BOTTOM_RIGHT,

	NONE
};

class UI_Transform : public Component
{
public:
	UI_Transform(C_UI* owner);
	~UI_Transform();

	virtual update_status Update(float dt);

	void OnInspector() override;

	void UpdateUITransformChilds();

	void RecalculateAnchor();

public:

	// Component reference
	C_UI* componentReference; 
	float auxPosX,auxPosY, anchorX, anchorY;

	UI_ANCHOR anchorType;

	float4x4 mMatrixUI;

};
#endif // __UI_TRANSFORM_H__