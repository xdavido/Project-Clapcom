#pragma once

#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/float4.h"

struct Color
{
	float r, g, b, a;
	
	Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f)
	{}

	Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a)
	{}

	void Set(float r, float g, float b, float a = 1.0f)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	void Set(float3 rgb, float a = 1.0f)
	{
		this->r = rgb.x;
		this->g = rgb.y;
		this->b = rgb.z;
		this->a = a;
	}

	void Set(float4 rgba)
	{
		this->r = rgba.x;
		this->g = rgba.y;
		this->b = rgba.z;
		this->a = rgba.w;
	}

	float* operator & ()
	{
		return (float*)this;
	}
};

extern Color Red;
extern Color Green;
extern Color Blue;
extern Color Black;
extern Color White;
extern Color Yellow;
extern Color Cyan;
extern Color Magenta;
extern Color LightGray;
extern Color MediumGray;
extern Color DarkGray;
extern Color Purple;
extern Color Orange;
extern Color Brown;