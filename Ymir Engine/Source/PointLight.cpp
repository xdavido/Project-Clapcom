#include "PointLight.h"

PointLight::PointLight() : Light(LightType::POINT_LIGHT, float3::one, 1.0f), radius(1.0f)
{
	collider = new CSphere(radius, 10);
	collider->wire = true;
}

PointLight::PointLight(float3 color, float intensity, float radius) : Light(LightType::POINT_LIGHT, color, intensity), radius(radius)
{
	collider = new CSphere(radius, 10);
	collider->wire = true;
}

PointLight::~PointLight()
{

}

void PointLight::Update()
{
	collider->radius = radius;
}

void PointLight::Render()
{
	collider->Render();
}

float PointLight::GetRadius() const 
{ 
	return radius; 
}

void PointLight::SetRadius(float radius) 
{ 
	this->radius = radius; 
}