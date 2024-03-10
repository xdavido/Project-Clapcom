#pragma once

#ifndef __DIRECTIONAL_LIGHT__
#define __DIRECTIONAL_LIGHT__

#include "LightManager.h"
#include "GameObject.h"
#include "External/MathGeoLib/include/Math/float3.h"

class DirectionalLight : GameObject {
public:

    DirectionalLight();
    virtual ~DirectionalLight();

    const LightType& GetType() const;

    const float3& GetColor() const;
    void SetColor(const float3& color);

    float GetRadius() const;
    void SetRadius(float radius);

    float GetIntensity() const;
    void SetIntensity(float intensity);

private:

    const LightType type;

    float3 color;

    float intensity;

};

#endif // __DIRECTIONAL_LIGHT__