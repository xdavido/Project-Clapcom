#include "Globals.h"
#include "ModulePhysics.h"

#include "Log.h"

#ifdef _DEBUG
	#pragma comment (lib, "Source/External/Bullet/libx86/BulletDynamics_debug.lib")
	#pragma comment (lib, "Source/External/Bullet/libx86/BulletCollision_debug.lib")
	#pragma comment (lib, "Source/External/Bullet/libx86/LinearMath_debug.lib")
#else					   
	#pragma comment (lib, "Source/External/Bullet/libx86/BulletDynamics.lib")
	#pragma comment (lib, "Source/External/Bullet/libx86/BulletCollision.lib")
	#pragma comment (lib, "Source/External/Bullet/libx86/LinearMath.lib")
#endif
 
ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	constraintSolver = new btSequentialImpulseConstraintSolver();
	broadphase = new btDbvtBroadphase();
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
}

ModulePhysics::~ModulePhysics() 
{
	delete dispatcher;
	delete collisionConfig;
	delete broadphase;
	delete constraintSolver;
}

// INIT ----------------------------------------------------------------------
bool ModulePhysics::Init() 
{
	LOG("Creating Physics Environment");
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, constraintSolver, collisionConfig);
	world->setGravity(GRAVITY);

	return true;
}

// START ---------------------------------------------------------------------
bool ModulePhysics::Start()
{
	return true;
}

// PRE-UPDATE ----------------------------------------------------------------
update_status ModulePhysics::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// UPDATE --------------------------------------------------------------------
update_status ModulePhysics::Update(float dt)
{
	return UPDATE_CONTINUE;
}

// POST-UPDATE ---------------------------------------------------------------
update_status ModulePhysics::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// CLEANUP -------------------------------------------------------------------
bool ModulePhysics::CleanUp()
{
	return true;
}