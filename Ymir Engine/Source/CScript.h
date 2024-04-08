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

	void ReloadComponent();

	void OnRecursiveUIDChange(std::map<uint, GameObject*> gameObjects);

	void OnInspector() override;

	//void SaveData(JSON_Object* nObj);
	//void LoadData(DEConfig& nObj);

	void SetField(MonoClassField* field, GameObject* value);
	void DropField(SerializedField& fieldName, const char* dropType);

	void LoadScriptData(std::string scriptName);

	void CollisionEnterCallback(bool isTrigger, GameObject* collidedGameObject);

	void CollisionStayCallback(bool isTrigger, GameObject* collidedGameObject);

	void CollisionExitCallback(bool isTrigger, GameObject* collidedGameObject);

	void OnClickButton();
	void OnHoverButton();

public:

	std::vector<std::string> methods;
	std::vector<SerializedField> fields;

	MonoMethod* updateMethod;
	MonoMethod* startMethod;
	MonoMethod* onCollisionEnterMethod;
	MonoMethod* onCollisionStayMethod;
	MonoMethod* onCollisionExitMethod;
	MonoMethod* onTriggerEnterMethod;
	MonoMethod* onTriggerExitMethod;
	MonoMethod* onClickButtonMethod;
	MonoMethod* onHoverButtonMethod;

	uint32_t noGCobject;
	std::string name;

	bool isStarting;
	static CScript* runningScript;
};