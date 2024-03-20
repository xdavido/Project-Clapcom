#pragma once

#ifndef __POINT_LIGHT__
#define __POINT_LIGHT__

#include "Light.h"

class PointLight : public Light {
public:

    PointLight();
    PointLight(float3 color, float intensity, float radius);

    virtual ~PointLight();

    void Update() override; 
    void Render() override;

    float GetRadius() const;
    void SetRadius(float radius);

private:

    CSphere* shape;
    float radius;

};

#endif // __POINT_LIGHT__