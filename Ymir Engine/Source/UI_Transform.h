#ifndef __UI_TRANSFORM_H__
#define __UI_TRANSFORM_H__

#pragma once

#include "C_UI.h"

class UI_Transform : public Component
{
public:
	UI_Transform(C_UI* owner);
	~UI_Transform();

	virtual update_status Update(float dt);

	void OnInspector() override;

public:

	// Component reference
	C_UI* componentReference;

	float4x4 mMatrixUI;


	bool dirty_;

};
#endif // __UI_TRANSFORM_H__