
#pragma once
#include "Color.h"
#include "External/MathGeoLib/include/Math/float3.h"

struct GL_Light
{
	GL_Light();

	void Init();
	void SetPos(float x, float y, float z);
	void Active(bool active);
	void Render();

	Color ambient;
	Color diffuse;
	float3 position;

	int ref;
	bool on;
};