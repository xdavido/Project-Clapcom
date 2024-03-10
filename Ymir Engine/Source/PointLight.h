#pragma once

#ifndef __POINT_LIGHT__
#define __POINT_LIGHT__

#include "LightManager.h"
#include "GameObject.h"
#include "External/MathGeoLib/include/Math/float3.h"

class PointLight : GameObject {
public:

    PointLight();
    virtual ~PointLight();

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

    float radius;
    float intensity;

};

#endif // __POINT_LIGHT__