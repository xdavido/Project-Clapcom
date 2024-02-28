#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"

#include "Log.h"

#include <vector>

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
	
	for (int i = 0; i < bodiesList.size(); i++)	
		RemoveBody(bodiesList[i]);

	for (int i = 0; i < collidersList.size(); i++) 
		RemoveCollider(collidersList[i]);
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
	if (TimeManager::gameTimer.GetState() == TimerState::RUNNING)
	{
		world->stepSimulation(dt, 15);
	}

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
																			  
// Module Functions	----------------------------------------------------------
void ModulePhysics::AddBody(btRigidBody* b)
{
	bodiesList.push_back(b);
	world->addRigidBody(b);
}

void ModulePhysics::AddCollider(btCollisionShape* c)
{
	collidersList.push_back(c);
	//world->addCollisionObject(c);
}

void ModulePhysics::RemoveBody(btRigidBody* b)
{
	world->removeRigidBody(b);

	bodiesList.erase(std::find(bodiesList.begin(), bodiesList.end(), b));
	bodiesList.shrink_to_fit();
}

void ModulePhysics::RemoveCollider(btCollisionShape* c)
{
	//world->removeCollisionObject(c);

	collidersList.erase(std::find(collidersList.begin(), collidersList.end(), c));
	collidersList.shrink_to_fit();
}