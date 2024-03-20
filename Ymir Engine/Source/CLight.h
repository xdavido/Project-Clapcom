#pragma once

#ifndef __COMPONENT_LIGHT__
#define __COMPONENT_LIGHT__

#include "Component.h"

#include "Light.h"
#include "ModuleLightManager.h"

class CLight : public Component {
public:

	CLight(GameObject* owner, Light* lightReference);
	virtual ~CLight();

	void Update() override;
	void OnInspector() override;

public:

	Light* lightReference;

};

#endif // __COMPONENT_LIGHT__