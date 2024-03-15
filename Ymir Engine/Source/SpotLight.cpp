#include "SpotLight.h"

SpotLight::SpotLight() : Light(LightType::SPOT_LIGHT, float3::one, 1.0f), range(1.0f), radius(1.0f)
{
	shape = new CCone(radius, 20, range);
	shape->wire = true;
}

SpotLight::SpotLight(float3 color, float intensity, float range, float radius) : Light(LightType::SPOT_LIGHT, color, intensity), range(range), radius(radius)
{
	shape = new CCone(radius, 20, range);
	shape->wire = true;
}

SpotLight::~SpotLight()
{
	delete shape;
}

void SpotLight::Update()
{
	shape->transform = lightGO->mTransform->mGlobalMatrix.Transposed();
	shape->color.Set(this->GetColor());

	shape->radius = radius;
	shape->height = range;
}

void SpotLight::Render()
{
	shape->Render(shape->color);
}

float SpotLight::GetRange() const
{
	return range;
}

void SpotLight::SetRange(float range)
{
	this->range = range;
}

void SpotLight::SetRadius(float radius)
{
	this->radius = radius;
}

float SpotLight::GetRadius() const
{
	return radius;
}