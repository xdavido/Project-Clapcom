#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"

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
	LOG("Creating ModulePhysics");

	// Physics simulation (world)
	constraintSolver = new btSequentialImpulseConstraintSolver();
	broadphase = new btDbvtBroadphase();
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);

	// Debug drawer	
	debugDraw = new DebugDrawer();
}

ModulePhysics::~ModulePhysics() 
{
	delete dispatcher;
	delete collisionConfig;
	delete broadphase;
	delete constraintSolver;

	delete debugDraw;
	
	for (int i = 0; i < bodiesList.size(); i++)	
		RemoveBody(bodiesList[i]);

	for (int i = 0; i < collidersList.size(); i++) 
		RemoveCollider(collidersList[i]);
}

// INIT ----------------------------------------------------------------------
bool ModulePhysics::Init() 
{
	debugDraw->setDebugMode(1);

	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, constraintSolver, collisionConfig);
	world->setGravity(GRAVITY);
	world->setDebugDrawer(debugDraw);

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
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		debug = !debug;
		
		if (debug) LOG("DebugDrawer On");
		else LOG("DebugDrawer Off");
	}

	if (debug == true)
	{
		world->debugDrawWorld();
	}
		
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


// DEBUG DRAWER =============================================
void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	line.origin.Set(from.getX(), from.getY(), from.getZ());
	line.destination.Set(to.getX(), to.getY(), to.getZ());
	line.color.Set(color.getX(), color.getY(), color.getZ());
	line.Render();
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	point.transform.Translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	point.color.Set(color.getX(), color.getY(), color.getZ());
	point.Render();
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("Bullet warning: %s", warningString);
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	LOG("Bullet draw text: %s", textString);
}

void DebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes)debugMode;
}

int	 DebugDrawer::getDebugMode() const
{
	return mode;
}

bool ModulePhysics::RayCast(const btVector3& from, const btVector3& to, btVector3& hitPoint)
{
	// Manejador de rayos (from = start, to = end)
	btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);

	// Realizar el raycast
	world->rayTest(from, to, rayCallback);

	// Comprobar si hubo una colisión
	if (rayCallback.hasHit()) {
		hitPoint = rayCallback.m_hitPointWorld; // Punto de impacto
		return true; // Hubo colisión
	}

	return false; 
}
