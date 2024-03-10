#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light(LightType::DIRECTIONAL_LIGHT, float3::one, 1.0f)
{

}

DirectionalLight::DirectionalLight(float3 color, float intensity) : Light(LightType::DIRECTIONAL_LIGHT, color, intensity)
{

}

DirectionalLight::~DirectionalLight()
{

}