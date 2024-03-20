#include "Component.h"
#include "GameObject.h"

#include "External/mmgr/mmgr.h"

Component::Component()
{
	mOwner = nullptr;
	ctype = ComponentType::NONE;

	active = true;

	UID = Random::Generate();
}

Component::Component(GameObject* owner, ComponentType type)
{
	mOwner = owner;
	ctype = type;

	active = true;

	UID = Random::Generate();
}

Component::~Component()
{
	mOwner = nullptr;
}

void Component::Enable()
{
	active = true;
}

void Component::Disable()
{
	active = false;
}

void Component::Update()
{

}

void Component::OnInspector()
{

}
