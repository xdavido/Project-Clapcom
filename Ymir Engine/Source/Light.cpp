#include "Light.h"

Light::Light() : type(LightType::UNKNOWN), color(float3::one), intensity(1.0f)
{
	lightGO = nullptr;
}

Light::Light(LightType type, float3 color, float intensity) : type(type), color(color), intensity(intensity)
{
	lightGO = nullptr;
}

Light::~Light()
{

}

void Light::Update()
{

}

void Light::Render()
{

}

const LightType& Light::GetType() const
{
	return type;
}

void Light::SetType(const LightType& type)
{
	this->type = type;
}

const float3& Light::GetColor() const
{
	return color;
}

void Light::SetColor(const float3& color)
{
	this->color = color;
}

float Light::GetIntensity() const
{
	return intensity;
}

void Light::SetIntensity(float intensity)
{
	this->intensity = intensity;
}
