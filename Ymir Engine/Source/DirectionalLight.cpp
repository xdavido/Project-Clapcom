#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light(LightType::DIRECTIONAL_LIGHT, float3::one, 1.0f)
{
	shape = new CCylinder(0.25, 3);
	shape->wire = true;
}

DirectionalLight::DirectionalLight(float3 color, float intensity) : Light(LightType::DIRECTIONAL_LIGHT, color, intensity)
{
	shape = new CCylinder(0.25, 3);
	shape->wire = true;
}

DirectionalLight::~DirectionalLight()
{
	delete shape;
}

void DirectionalLight::Update()
{
	shape->transform = lightGO->mTransform->mGlobalMatrix.Transposed();
	shape->color.Set(this->GetColor());
}

void DirectionalLight::Render()
{
	shape->Render(shape->color);
}
