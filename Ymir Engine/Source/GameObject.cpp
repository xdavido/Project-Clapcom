#include "GameObject.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleFileSystem.h"
#include "PhysfsEncapsule.h"
#include "CScript.h"
#include "Random.h"
#include "ModuleEditor.h"

#include "External/mmgr/mmgr.h"

GameObject::GameObject()
{
	this->name = "";
	mParent = nullptr;

	active = true;
	selected = false;
	pendingToDelete = false;
	hidden = false;

	mTransform = nullptr;

	UID = Random::Generate();
}

GameObject::GameObject(std::string name, GameObject* parent)
{
	this->name = name;
	mParent = parent;

	active = true;
	selected = false;
	pendingToDelete = false;
	hidden = false;

	mTransform = nullptr;
	UID = Random::Generate();

	if (mParent != nullptr)
	{
		mTransform = new CTransform(this);
		AddComponent(mTransform);
	}
}

GameObject::~GameObject()
{
	// Clear components
	ClearVecPtr(mComponents);

	mTransform = nullptr;

	if (!mChildren.empty())
	{
		ClearVecPtr(mChildren);
	}
	for (size_t i = 0; i < csReferences.size(); i++)
	{
		mono_field_set_value(mono_gchandle_get_target(csReferences[i]->parentSC->noGCobject), csReferences[i]->field, NULL);
		csReferences[i]->fiValue.goValue = nullptr;
	}
	csReferences.clear();

	auto it = std::find(External->scene->gameObjects.begin(), External->scene->gameObjects.end(), this);
	if (it != External->scene->gameObjects.end()) {
		External->scene->gameObjects.erase(it);
		
	}

}

update_status GameObject::Update(float dt)
{
	// Check if any of the UIDs is repeated (it's not gonna happen)
	
	// FRANCESC: This shouldn't be commented, but it would need a rework in the future 
	// (separate gameobject and resourcemesh UID)

	for (auto it = External->scene->gameObjects.begin(); it != External->scene->gameObjects.end(); ++it) {

		if ((*it)->UID == this->UID && (*it) != this) { // If it is repeated, regenerate

			this->UID = Random::Generate();

		}

		if ((*it)->name == this->name && (*it) != this) { // If it is repeated, regenerate

			this->name = External->scene->GetUniqueName(this->name);

		}

	}

	return update_status::UPDATE_CONTINUE;
}

void GameObject::Enable()
{
	if (!active) {
		active = true;
	}
}

void GameObject::Disable()
{
	if (active) {
		active = false;
	}
}

GameObject* GameObject::FindChild(uint UID_ToFind, GameObject* go)
{
	for (auto i = 0; i < mChildren.size(); i++)
	{
		if (go != nullptr)
		{
			break;
		}
		if (!mChildren[i]->mChildren.empty())
		{
			go = mChildren[i]->FindChild(UID_ToFind, go);
		}
		if (UID_ToFind == mChildren[i]->UID)
		{
			go = mChildren[i];
			return go;
		}
	}

	return go;
}

GameObject* GameObject::GetChildByUID(const uint& UID)
{
	GameObject* gameObjectWithUID = nullptr;

	for (auto it = mChildren.begin(); it != mChildren.end(); ++it) {

		if ((*it)->UID == UID) {

			gameObjectWithUID = (*it);

		}

	}

	return gameObjectWithUID;
}

void GameObject::SetParent(GameObject* newParent)
{
	if (this->mParent)
	{
		auto it = std::find(this->mParent->mChildren.begin(), this->mParent->mChildren.end(), this);
		if (it != this->mParent->mChildren.end())
		{
			this->mParent->mChildren.erase(it);
		}
	}

	mParent = newParent;
	mParent->AddChild(this);

}

void GameObject::ReParent(GameObject* newParent)
{
	mParent->RemoveChild(this);
	mParent = newParent;
	mParent->mChildren.push_back(this);

	//Update transform values
	if (mParent->mTransform != nullptr)
	{
		mTransform->ReparentTransform(mParent->mTransform->mGlobalMatrix.Inverted() * mTransform->mGlobalMatrix);
	}

	else
	{
		mTransform->ReparentTransform(mTransform->mGlobalMatrix);
	}

}

void GameObject::AddChild(GameObject* child)
{
	mChildren.push_back(child);
}

void GameObject::DeleteChild(GameObject* go)
{
	RemoveChild(go);
	go->ClearReferences();

	RELEASE(go);
}

void GameObject::RemoveChild(GameObject* go)
{
	mChildren.erase(std::find(mChildren.begin(), mChildren.end(), go));
	mChildren.shrink_to_fit();
}

void GameObject::SwapChildren(GameObject* go)
{
	int index = std::find(mParent->mChildren.begin(), mParent->mChildren.end(), this) - mParent->mChildren.begin();
	int index2 = std::find(go->mParent->mChildren.begin(), go->mParent->mChildren.end(), go) - go->mParent->mChildren.begin();

	GameObject* aux = go->mParent;
	go->ReParent(mParent);
	this->ReParent(aux);

	if (index < index2)
	{
		Swap(go->mParent->mChildren, std::find(go->mParent->mChildren.begin(), go->mParent->mChildren.end(), go) - go->mParent->mChildren.begin(), index);
		Swap(mParent->mChildren, std::find(mParent->mChildren.begin(), mParent->mChildren.end(), this) - mParent->mChildren.begin(), index2);
	}
	else
	{
		Swap(mParent->mChildren, std::find(mParent->mChildren.begin(), mParent->mChildren.end(), this) - mParent->mChildren.begin(), index2);
		Swap(go->mParent->mChildren, std::find(go->mParent->mChildren.begin(), go->mParent->mChildren.end(), go) - go->mParent->mChildren.begin(), index);
	}

}

void GameObject::AddComponent(Component* component)
{
	mComponents.push_back(component);
}

bool GameObject::AddComponent(ComponentType ctype, void* var)
{
	Component* temp;
	bool ret = true;

	switch (ctype)
	{
	case ComponentType::TRANSFORM:
		if (mTransform == nullptr)
		{
			temp = new CTransform(this, float3(0, 0, 0), Quat(0, 0, 0, 0), float3(1, 1, 1));
			mTransform = (CTransform*)temp;
			mComponents.push_back(mTransform);
		}
		else { ret = false; }
		break;
	case ComponentType::MESH:
		// A gameObject can't have more than one mesh
		if (GetComponent(ComponentType::MESH) == nullptr)
		{
			temp = new CMesh(this);
			mComponents.push_back(temp);
		}
		else { ret = false; }
		break;
	case ComponentType::MATERIAL:
		// A gameObject can't have more than one material
		// In unity there can be more than one if embeded (?) see snowman for reference 
		if (GetComponent(ComponentType::MATERIAL) == nullptr)
		{
			temp = new CMaterial(this);
			mComponents.push_back(temp);
		}
		else { ret = false; }
		break;
	case ComponentType::CAMERA:
		if (GetComponent(ComponentType::CAMERA) == nullptr)
		{
			temp = new CCamera(this);
			mComponents.push_back(temp);
		}
		else { ret = false; }
		break;
	case ComponentType::PHYSICS:
		if (GetComponent(ComponentType::PHYSICS) == nullptr)
		{
			temp = new CCollider(this);
			mComponents.push_back(temp);
		}
		else { ret = false; }
		break;
	case ComponentType::SCRIPT:
	{
		temp = new CScript(this, External->editor->script_name.c_str());
		mComponents.push_back(temp);
	}
	break;

	case ComponentType::AUDIO_SOURCE:
		if (GetComponent(ComponentType::AUDIO_SOURCE) == nullptr)
		{
			temp = new CAudioSource(this);
			mComponents.push_back(temp);
		}
		else { ret = false; }
		break;
	case ComponentType::AUDIO_LISTENER:
		if (GetComponent(ComponentType::AUDIO_LISTENER) == nullptr)
		{
			temp = new CAudioListener(this);
			mComponents.push_back(temp);
		}
		else { ret = false; }
		break;
	default:
		break;
	}

	temp = nullptr;
	return ret;
}

Component* GameObject::GetComponent(ComponentType ctype)
{
	for (auto it = mComponents.begin(); it != mComponents.end(); ++it) {

		if ((*it)->ctype == ctype) {

			return (*it);

		}

	}

	return nullptr;
}
Component* GameObject::GetComponent(ComponentType ctype,char* scriptname)
{
	 // Suponiendo que ya tienes asignado un valor a scriptname
	char* concatenatedName = new char[strlen(scriptname) + 4]; // +4 para ".cs" y el terminador nulo
	strcpy(concatenatedName, scriptname);
	strcat(concatenatedName, ".cs");
	for (size_t i = 0; i < mComponents.size(); i++)
	{
		if (mComponents[i] && mComponents[i]->ctype == ctype)
		{
			if (ctype == ComponentType::SCRIPT)
			{
				CScript* a = dynamic_cast<CScript*>(mComponents[i]);
				if (scriptname != nullptr && strcmp(a->name.c_str(), concatenatedName) == 0)
					return mComponents[i];
			}
			else
			{
				return mComponents[i];
			}
		}
	}

	return nullptr;
}

std::vector<Component*> GameObject::GetAllComponentsByType(ComponentType type)
{
	std::vector<Component*> vec = {};

	for (auto i = 0; i < mComponents.size(); i++)
	{
		if (mComponents[i]->ctype == type)
		{
			vec.push_back(mComponents[i]);
		}
	}
	
	return vec;
}

void GameObject::RemoveComponent(Component* component)
{
	if (!mComponents.empty() && component != nullptr)
	{
		mComponents.erase(std::find(mComponents.begin(), mComponents.end(), component));

		RELEASE(component);
	}
}

void GameObject::RemoveCSReference(SerializedField* fieldToRemove)
{
	for (size_t i = 0; i < csReferences.size(); ++i)
	{
		if (csReferences[i]->fiValue.goValue == fieldToRemove->fiValue.goValue)
		{
			mono_field_set_value(mono_gchandle_get_target(csReferences[i]->parentSC->noGCobject), csReferences[i]->field, NULL);
			csReferences.erase(csReferences.begin() + i);
		}
	}
}

void GameObject::CollectChilds(std::vector<GameObject*>& vector)
{
	vector.push_back(this);
	for (uint i = 0; i < mChildren.size(); i++)
		mChildren[i]->CollectChilds(vector);
}

void GameObject::DestroyGameObject()
{
	pendingToDelete = true;
}

GameObject* GameObject::GetGameObjectFromUID(const std::vector<GameObject*>& gameObjects, const uint& UID)
{
	GameObject* gameObjectWithUID = nullptr;

	for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it) {

		if ((*it)->UID == UID) {

			gameObjectWithUID = (*it);

		}

	}

	return gameObjectWithUID;
}

bool GameObject::CompareTag(std::string _tag)
{
	if (tag == _tag)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

//
void GameObject::RemoveReference(Component* comp)
{
	if (!vReferences.empty())
	{
		vReferences.erase(std::find(vReferences.begin(), vReferences.end(), comp));
		vReferences.shrink_to_fit();
	}
}

void GameObject::ClearReferences()
{
	// Clear references
	for (auto it = vReferences.begin(); it != vReferences.end(); ++it)
	{
		(*it)->OnReferenceDestroyed(this);
	}

	ClearVec(vReferences);
}