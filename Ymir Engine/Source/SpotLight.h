#pragma once

#ifndef __SPOT_LIGHT__
#define __SPOT_LIGHT__

#include "Light.h"

class SpotLight : public Light {
public:

    SpotLight();
    SpotLight(float3 color, float intensity, float range, float angle);

    virtual ~SpotLight();

    void Update() override;
    void Render() override;

    float GetRange() const;
    void SetRange(float range);

    float GetAngle() const;
    void SetAngle(float angle);

private:

    float range;
    float angle;

    CCone* shape;

};

#endif // __SPOT_LIGHT__