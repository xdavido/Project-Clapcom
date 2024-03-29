#pragma once
#include "Globals.h"

#include <string>
#include <map>

class GameObject;

enum ComponentType {

	NONE = -1,

	TRANSFORM,
	MESH,
	MATERIAL,
	SCRIPT,
	AUDIO_SOURCE,
	AUDIO_LISTENER,
	CAMERA,
	RIGIDBODY,
	PHYSICS,
	ANIMATION,
	PARTICLE,
	UI_TRAMSFORM,
	UI,
	LIGHT,

	ALL_TYPES

};

class Component {
public:

	Component();
	Component(GameObject* owner, ComponentType type);
	virtual ~Component();

	void Enable(); 
	void Disable();

	virtual void Update();
	virtual void OnInspector();

	uint GetUID();

	/// <summary>
	/// Create Button Drag&Drop target. If GO is dropped, set as new reference to given pointer (go)
	/// </summary>
	/// <param name="go ->"> pointer to change </param>
	/// <param name="buttonClicked ->"> optional parameter, if != nullptr --> become true on button click </param>
	/// <returns></returns>
	GameObject* ImGui_GameObjectReference(GameObject* go, bool *buttonClicked = nullptr);

	virtual void SetReference();
	virtual void OnReferenceDestroyed(void* ptr = nullptr);

public:

	GameObject* mOwner;

	bool active;
	ComponentType ctype;

	// DO NOT USE, Save/Load purposes only
	std::map<std::string, int> vTempReferences;

protected:

	uint UID;
};