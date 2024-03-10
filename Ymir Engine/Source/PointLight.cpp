#include "PointLight.h"

PointLight::PointLight() : type(LightType::POINT_LIGHT), color(float3::one), radius(1.0f), intensity(1.0f)
{

}

PointLight::~PointLight()
{

}

const LightType& PointLight::GetType() const
{
	return type;
}

const float3& PointLight::GetColor() const 
{ 
	return color;
}

void PointLight::SetColor(const float3& color) 
{ 
	this->color = color; 
}

float PointLight::GetRadius() const 
{ 
	return radius; 
}

void PointLight::SetRadius(float radius) 
{ 
	this->radius = radius; 
}

float PointLight::GetIntensity() const 
{ 
	return intensity; 
}

void PointLight::SetIntensity(float intensity) 
{ 
	this->intensity = intensity; 
}