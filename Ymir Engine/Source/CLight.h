#pragma once

#ifndef __COMPONENT_LIGHT__
#define __COMPONENT_LIGHT__

#include "Component.h"

#include "Light.h"
#include "ModuleLightManager.h"

class CLight : public Component {
public:

	CLight(GameObject* owner, LightType type);
	virtual ~CLight();

	void Update() override;
	void OnInspector() override;

public:

	Light* lightOwner;
	LightType type;
};

#endif // __COMPONENT_LIGHT__