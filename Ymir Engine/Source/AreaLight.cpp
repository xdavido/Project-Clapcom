#include "AreaLight.h"

AreaLight::AreaLight() : Light(LightType::AREA_LIGHT, float3::one, 1.0f), range(1.0f), width(1.0f), height(1.0f)
{

}

AreaLight::AreaLight(float3 color, float intensity, float range, float width, float height) : Light(LightType::AREA_LIGHT, color, intensity), range(range), width(width), height(height)
{

}

AreaLight::~AreaLight()
{

}

float AreaLight::GetRange() const
{
	return range;
}

void AreaLight::SetRange(float range)
{
	this->range = range;
}

void AreaLight::SetWidth(float width)
{
	this->width = width;
}

float AreaLight::GetWidth() const
{
	return width;
}

void AreaLight::SetHeight(float height)
{
	this->height = height;
}

float AreaLight::GetHeight() const
{
	return height;
}