#include "GameObject.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleFileSystem.h"
#include "PhysfsEncapsule.h"

GameObject::GameObject()
{
	this->name = "";
	mParent = nullptr;

	active = true;
	selected = false;
	hidden = false;

	mTransform = nullptr;
}

GameObject::GameObject(std::string name, GameObject* parent)
{
	this->name = name;
	mParent = parent;

	active = true;
	selected = false;
	hidden = false;

	mTransform = nullptr;

	if (mParent != nullptr)
	{
		mTransform = new CTransform(this);
		AddComponent(mTransform);
	}
}

GameObject::~GameObject()
{
	ClearVecPtr(mComponents);

	mTransform = nullptr;

	if (!mChildren.empty())
	{
		ClearVecPtr(mChildren);
	}
}

update_status GameObject::Update(float dt)
{
	// Check if any of the UIDs is repeated (it's not gonna happen)

	for (auto it = External->scene->gameObjects.begin(); it != External->scene->gameObjects.end(); ++it) {

		if ((*it)->UID == this->UID && (*it) != this) { // If it is repeated, regenerate

			this->UID = Random::Generate();

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

	// Update transform values, there is a bug where if the parent has a different scale it changes the scale of the children
	if (mParent->mTransform != nullptr)
	{
		mTransform->ReparentTransform(mParent->mTransform->mGlobalMatrix.Inverted() * mTransform->mGlobalMatrix);
	}

	else
	{
		mTransform->ReparentTransform(mTransform->mGlobalMatrix);
	}
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
	RELEASE(go);
}

void GameObject::RemoveChild(GameObject* go)
{
	mChildren.erase(std::find(mChildren.begin(), mChildren.end(), go));
	mChildren.shrink_to_fit();
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

void GameObject::RemoveComponent(Component* component)
{
	// TODO: Sara --> test if it needs something else
	if (!mComponents.empty() && component != nullptr)
	{
		mComponents.erase(std::find(mComponents.begin(), mComponents.end(), component));
		
		RELEASE(component);
	}
}

void GameObject::DestroyGameObject()
{
	if (!mComponents.empty() && component != nullptr)
	{
		mComponents.erase(std::find(mComponents.begin(), mComponents.end(), component));
		//component->~Component();
		
		switch (component->ctype)
		{
		case NONE:
			break;
		case TRANSFORM:
		{
			mTransform = nullptr;
		}
		break;
		case MESH:
			break;
		case MATERIAL:
			break;
		case CAMERA:
			break;
		default:
			break;
		}

		RELEASE(component);
	}
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


