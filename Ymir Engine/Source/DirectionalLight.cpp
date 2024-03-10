#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : type(LightType::DIRECTIONAL_LIGHT), color(float3::one), intensity(1.0f)
{

}

DirectionalLight::~DirectionalLight()
{

}

const LightType& DirectionalLight::GetType() const
{
	return type;
}

const float3& DirectionalLight::GetColor() const
{
	return color;
}

void DirectionalLight::SetColor(const float3& color)
{
	this->color = color;
}

float DirectionalLight::GetIntensity() const
{
	return intensity;
}

void DirectionalLight::SetIntensity(float intensity)
{
	this->intensity = intensity;
}