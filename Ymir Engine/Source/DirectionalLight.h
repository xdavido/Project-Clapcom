#pragma once

#ifndef __DIRECTIONAL_LIGHT__
#define __DIRECTIONAL_LIGHT__

#include "Light.h"

class DirectionalLight : public Light {
public:

    DirectionalLight();
    DirectionalLight(float3 color, float intensity);

    virtual ~DirectionalLight();

private:

    

};

#endif // __DIRECTIONAL_LIGHT__