#pragma once

#ifndef __SPOT_LIGHT__
#define __SPOT_LIGHT__

#include "Light.h"

class SpotLight : public Light {
public:

    SpotLight();
    SpotLight(float3 color, float intensity, float range, float radius);

    virtual ~SpotLight();

    void Update() override;
    void Render() override;

    float GetRange() const;
    void SetRange(float range);

    float GetRadius() const;
    void SetRadius(float radius);

private:

    float range;
    float radius;
    
    CCone* shape;

};

#endif // __SPOT_LIGHT__