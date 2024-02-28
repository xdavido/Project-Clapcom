#pragma once	

#include "Component.h"
#include "Globals.h"

class CTransform;

class CAudioListener : public Component {
public:

	CAudioListener(GameObject* owner, bool defaultListener = false);
	virtual ~CAudioListener();

	void OnInspector() override;

	void Update() override;

	void SetVolume(float newVol);

	uint GetID();
	void SetID(uint id);

	void SetAsDefaultListener(bool setDefault = true);
	bool IsDefaultListener();

private:

	CTransform* myTransform;

	bool isDefaultListener;
	unsigned int id;

};
