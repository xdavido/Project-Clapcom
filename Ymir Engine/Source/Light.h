#pragma once

#ifndef __LIGHT__
#define __LIGHT__

#include "GameObject.h"

enum class LightType {

	UNKNOWN = -1,

	POINT_LIGHT,
	DIRECTIONAL_LIGHT,
	SPOT_LIGHT,
	AREA_LIGHT,

	ALL_TYPES

};

class Light : public GameObject {
public:

	Light();
	Light(LightType type, float3 color, float intensity);

	virtual ~Light();

	const LightType& GetType() const;
	void SetType(const LightType& type);

	const float3& GetColor() const;
	void SetColor(const float3& color);

	float GetIntensity() const;
	void SetIntensity(float intensity);

private:

	LightType type;

	float3 color;
	float intensity;

};

#endif // __LIGHT__