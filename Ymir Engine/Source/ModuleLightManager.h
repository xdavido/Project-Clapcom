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

	void EnableLightDebug(bool enable);
	bool IsLightDebugEnabled();

	Light* CreateLight(LightType type);

public:

	std::vector<Light*> lights;

private:

	bool debuglightsEnabled;

};

#endif // __MODULE_LIGHT_MANAGER__