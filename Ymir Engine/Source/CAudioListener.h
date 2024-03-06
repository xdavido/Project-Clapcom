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

	//void SaveData(JSON_Object* nObj) override;
	//void LoadData(DEConfig& nObj) override;

	void SetVolume(float newVol);

	uint GetID();
	void SetID(uint id);

	void SetAsDefaultListener(bool setDefault = true);
	bool IsDefaultListener();

	bool isDefaultListener;

private:

	CTransform* myTransform;
	
	unsigned int id;

};
