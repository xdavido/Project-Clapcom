#pragma once

#ifndef __AREA_LIGHT__
#define __AREA_LIGHT__

#include "Light.h"

class AreaLight : public Light {
public:

    AreaLight();
    AreaLight(float3 color, float intensity, float range, float width, float height);

    virtual ~AreaLight();

    float GetRange() const;
    void SetRange(float range);

    float GetWidth() const;
    void SetWidth(float width);

    float GetHeight() const;
    void SetHeight(float height);

private:

    float range;

    float width;
    float height;

};

#endif // __AREA_LIGHT__