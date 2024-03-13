#include "SpotLight.h"

SpotLight::SpotLight() : Light(LightType::SPOT_LIGHT, float3::one, 1.0f), range(1.0f), angle(1.0f)
{
	shape = new CCone(angle, 10, range);
	shape->wire = true;
}

SpotLight::SpotLight(float3 color, float intensity, float range, float angle) : Light(LightType::SPOT_LIGHT, color, intensity), range(range), angle(angle)
{
	shape = new CCone(angle, 10, range);
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

	shape->radius = angle;
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

void SpotLight::SetAngle(float angle)
{
	this->angle = angle;
}

float SpotLight::GetAngle() const
{
	return angle;
}