#include "AreaLight.h"

AreaLight::AreaLight() : Light(LightType::AREA_LIGHT, float3::one, 1.0f), range(1.0f), width(1.0f), height(1.0f)
{
	shape = new CCube(height, range, width);
	shape->wire = true;
}

AreaLight::AreaLight(float3 color, float intensity, float range, float width, float height) : Light(LightType::AREA_LIGHT, color, intensity), range(range), width(width), height(height)
{
	shape = new CCube(height, range, width);
	shape->wire = true;
}

AreaLight::~AreaLight()
{
	delete shape;
}

void AreaLight::Update()
{
	shape->transform = lightGO->mTransform->mGlobalMatrix.Transposed();
	shape->color.Set(this->GetColor());

	shape->size.x = height;
	shape->size.y = range;
	shape->size.z = width;
}

void AreaLight::Render()
{
	shape->Render(shape->color);
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