#pragma once

#include "Component.h"

#include<vector>
#include<string>

#include "External/mono/metadata/object-forward.h"
#include "ModuleMonoManager.h"

class CScript : public Component {
public:

	CScript(GameObject* _gm, const char* scriptName);
	virtual ~CScript();

	void Update() override;

	void OnInspector() override;

	//void SaveData(JSON_Object* nObj) override;
	//void LoadData(DEConfig& nObj) override;

	void SetField(MonoClassField* field, GameObject* value);
	void DropField(SerializedField& fieldName, const char* dropType);

	void LoadScriptData(const char*);

public:

};