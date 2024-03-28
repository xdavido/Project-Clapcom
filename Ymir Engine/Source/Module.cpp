#include "Module.h"

#include "External/mmgr/mmgr.h"
#include "Log.h"
	
Module::Module(Application* parent, bool start_enabled) : App(parent)
{

}

Module::~Module()
{

}

bool Module::Init()
{
	return true;
}

bool Module::Start()
{
	return true;
}

update_status Module::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status Module::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status Module::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool Module::CleanUp()
{
	return true;
}

void Module::OnCollision(PhysBody* body1, PhysBody* body2)
{
}