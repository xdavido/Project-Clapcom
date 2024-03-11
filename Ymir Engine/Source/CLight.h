#pragma once

#ifndef __COMPONENT_LIGHT__
#define __COMPONENT_LIGHT__

#include "Component.h"
#include "Light.h"

class CLight : public Component {
public:

	CLight(GameObject* owner);
	virtual ~CLight();

	void Update() override;
	void OnInspector() override;

public:

	Light* lightOwner;

};

#endif // __COMPONENT_LIGHT__