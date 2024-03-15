#pragma once
#include "Globals.h"

class GameObject;

enum ComponentType {

	NONE = -1,

	UI_TRAMSFORM,
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
	UI,

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

	/// <summary>
	/// Create Button Drag&Drop target. If GO is dropped, set as new reference to given pointer (go)
	/// </summary>
	/// <param name="go ->"> pointer to change </param>
	/// <param name="buttonClicked ->"> optional parameter, if != nullptr --> become true on button click </param>
	/// <returns></returns>
	GameObject* ImGui_GameObjectReference(GameObject* go, bool *buttonClicked = nullptr);

	virtual void SetReference(GameObject& go);
	virtual void OnReferenceDestroyed(void* ptr = nullptr);

public:

	GameObject* mOwner;

	bool active;
	ComponentType ctype;

protected:

	uint UID;
};