#pragma once

#ifndef __MODULE_LIGHT_MANAGER__
#define __MODULE_LIGHT_MANAGER__

#include "Module.h"
#include "Light.h"
#include "CLight.h"

#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "AreaLight.h"

class ModuleLightManager : public Module {
public:

	ModuleLightManager(Application* app, bool start_enabled = true);
	virtual ~ModuleLightManager();

	//void EnableLightingSystem(bool enable);
	Light* CreateLight(LightType type = LightType::UNKNOWN);

private:

	std::vector<Light*> lights;

};

#endif // __MODULE_LIGHT_MANAGER__