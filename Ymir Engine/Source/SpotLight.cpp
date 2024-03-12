#include "SpotLight.h"

SpotLight::SpotLight() : Light(LightType::SPOT_LIGHT, float3::one, 1.0f), range(1.0f), angle(1.0f)
{

}

SpotLight::SpotLight(float3 color, float intensity, float range, float angle) : Light(LightType::SPOT_LIGHT, color, intensity), range(range), angle(angle)
{

}

SpotLight::~SpotLight()
{

}

void SpotLight::Update()
{

}

void SpotLight::Render()
{

}

float SpotLight::GetRange() const
{
	return range;
}

void SpotLight::SetRange(float range)
{
	this->range = range;
}

void SpotLight::SetAngle(float angle)
{
	this->angle = angle;
}

float SpotLight::GetAngle() const
{
	return angle;
}