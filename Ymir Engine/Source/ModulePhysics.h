#pragma once
#include "Module.h"
#include "Globals.h"

#include "External/Bullet/include/btBulletDynamicsCommon.h"
#include "External/glmath.h"

#include <vector>

class ModulePhysics : public Module 
{
public:

	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

private:

	btDynamicsWorld* world; // World se puede dejar en public si se requiere
	btDispatcher* dispatcher;
	btCollisionConfiguration* collisionConfig;
	btBroadphaseInterface* broadphase;
	btConstraintSolver* constraintSolver;
};