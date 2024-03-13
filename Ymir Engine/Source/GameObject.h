#pragma once

#include <vector>
#include <string>

#include "Globals.h"

#include "Model.h"
#include "Component.h"
#include "CTransform.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CCamera.h"
#include "CAudioListener.h"
#include "CAudioSource.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CAnimation.h"
#include "CScript.h"
#include "Component.h"

#include "Random.h"

struct SerializedField;

class GameObject {
public:

	GameObject();
	GameObject(std::string name, GameObject* parent);
	virtual ~GameObject();

	virtual update_status Update(float dt);

	void Enable();
	void Disable();

	//---Parent/Child---
	void SetParent(GameObject* newParent);
	void ReParent(GameObject* newParent);
	GameObject* GetChildByUID(const uint& UID);
	void AddChild(GameObject* go);
	void DeleteChild(GameObject* go);
	//Remove from children vector (do not use)
	void RemoveChild(GameObject* go);

	void AddComponent(Component* component);
	bool AddComponent(ComponentType ctype, void* var = NULL);
	Component* GetComponent(ComponentType ctype);
	std::vector<Component*> GetAllComponentsByType(ComponentType type);
	void RemoveComponent(Component* component);

	void DestroyGameObject();

	void CollectChilds(std::vector<GameObject*>& vector);

	static GameObject* GetGameObjectFromUID(const std::vector<GameObject*>& gameObjects, const uint& UID);

	bool CompareTag(const char* _tag);

	// Clear references
	void ClearReferences();
	void RemoveReference(Component* comp);

public:

	std::string name;
	uint UID;

	std::string type = "None";

	GameObject* mParent;
	std::vector<GameObject*> mChildren;
	std::vector<Component*> mComponents;

	std::vector<SerializedField*> csReferences;

	CTransform* mTransform; 
	bool pendingToDelete;

	bool active;
	bool selected;
	bool hidden;

	char tag[32] = "Untagged";

	std::vector<Component*> vReferences;
};