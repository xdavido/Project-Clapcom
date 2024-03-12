#include "PointLight.h"

PointLight::PointLight() : Light(LightType::POINT_LIGHT, float3::one, 1.0f), radius(1.0f)
{
	shape = new CSphere(radius, 10);
	shape->wire = true;
}

PointLight::PointLight(float3 color, float intensity, float radius) : Light(LightType::POINT_LIGHT, color, intensity), radius(radius)
{
	shape = new CSphere(radius, 10);
}

PointLight::~PointLight()
{
	delete shape;
}

void PointLight::Update()
{
	shape->transform = lightGO->mTransform->mGlobalMatrix.Transposed();
	shape->color.Set(this->GetColor());
	shape->radius = radius;
}

void PointLight::Render()
{
	shape->Render(shape->color);
}

float PointLight::GetRadius() const 
{ 
	return radius; 
}

void PointLight::SetRadius(float radius) 
{ 
	this->radius = radius; 
}