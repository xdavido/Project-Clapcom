#pragma once

#ifndef __LIGHT__
#define __LIGHT__

#include "GameObject.h"
#include "Primitive.h"

enum class LightType {

	UNKNOWN = -1,

	POINT_LIGHT,
	DIRECTIONAL_LIGHT,
	SPOT_LIGHT,
	AREA_LIGHT,

	ALL_TYPES

};

class Light {
public:

	Light();
	Light(LightType type, float3 color, float intensity);

	virtual ~Light();

	virtual void Update();
	virtual void Render();

	const LightType& GetType() const;
	void SetType(const LightType& type);

	const float3& GetColor() const;
	void SetColor(const float3& color);

	float GetIntensity() const;
	void SetIntensity(float intensity);

public:

	GameObject* lightGO;

	bool debug = true;

private:

	LightType type;

	float3 color;
	float intensity;

};

#endif // __LIGHT__