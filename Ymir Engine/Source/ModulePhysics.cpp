#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "PhysBody.h"

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
 
#include "External/mmgr/mmgr.h"

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
	debug = true;

	//Colors
	colliderColor = Green;
}

ModulePhysics::~ModulePhysics() 
{
	delete dispatcher;
	delete collisionConfig;
	delete broadphase;
	delete constraintSolver;

	delete debugDraw;
	
	//for (int i = 0; i < bodiesList.size(); i++)	
	//	RemoveBody(bodiesList[i]);

	//for (int i = 0; i < collidersList.size(); i++) 
	//	RemoveCollider(collidersList[i]);
}

// INIT ----------------------------------------------------------------------
bool ModulePhysics::Init() 
{
	debugDraw->setDebugMode(1);

	return true;
}

// START ---------------------------------------------------------------------
bool ModulePhysics::Start()
{
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, constraintSolver, collisionConfig);
	world->setGravity(GRAVITY);
	world->setDebugDrawer(debugDraw);

	return true;
}

// PRE-UPDATE ----------------------------------------------------------------
update_status ModulePhysics::PreUpdate(float dt)
{
	//world->stepSimulation(dt, 15);

	return UPDATE_CONTINUE;
}

// UPDATE --------------------------------------------------------------------
update_status ModulePhysics::Update(float dt)
{
	if (TimeManager::gameTimer.GetState() == TimerState::RUNNING)
	{
		world->stepSimulation(dt, 15);
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
//void ModulePhysics::AddBody(btRigidBody* b)
//{
//	bodiesList.push_back(b);
//	world->addRigidBody(b);
//}

//void ModulePhysics::AddCollider(btCollisionShape* c)
//{
//	collidersList.push_back(c);
//	//world->addCollisionObject(c);
//}

//void ModulePhysics::RemoveCollider(btCollisionShape* c)
//{
//	//world->removeCollisionObject(c);
//
//	collidersList.erase(std::find(collidersList.begin(), collidersList.end(), c));
//	collidersList.shrink_to_fit();
//}

// ADDBODY ============================================================================================================
// Box Collider -------------------------------------------------------------------------------------------------------
PhysBody* ModulePhysics::AddBody(CCube cube, physicsType physType, float mass, bool gravity, btCollisionShape*& shape)
{
	shape = new btBoxShape(btVector3(cube.size.x * 0.5f, cube.size.y * 0.5f, cube.size.z * 0.5f));
	collidersList.push_back(shape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(getOpenGLMatrix(cube.transform));

	btVector3 localInertia(0, 0, 0);
	
	if (mass != 0.f)
		shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.push_back(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody* pbody = new PhysBody(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodiesList.push_back(pbody);

	return pbody;
}

// Sphere ---------------------------------------------------------------------------------------------------------------
PhysBody* ModulePhysics::AddBody(CSphere sphere, physicsType physType, float mass, bool gravity, btCollisionShape*& shape)
{
	shape = new btSphereShape(sphere.radius);
	collidersList.push_back(shape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(getOpenGLMatrix(sphere.transform));

	btVector3 localInertia(0, 0, 0);

	if (mass != 0.f)
		shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.push_back(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody* pbody = new PhysBody(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodiesList.push_back(pbody);

	return pbody;
}

// Capsule --------------------------------------------------------------------------------------------------------------
PhysBody* ModulePhysics::AddBody(CCapsule capsule, physicsType physType, float mass, bool gravity, btCollisionShape*& shape)
{
	shape = new btCapsuleShape(capsule.height, capsule.radius);
	collidersList.push_back(shape);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(getOpenGLMatrix(capsule.transform));

	btVector3 localInertia(0, 0, 0);

	if (mass != 0.f)
		shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.push_back(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody* pbody = new PhysBody(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodiesList.push_back(pbody);

	return pbody;
}

// Convex Collider ----------------------------------------------------------------------------------------------------
PhysBody* ModulePhysics::AddBody(CMesh* mesh, physicsType, float mass, bool gravity, btConvexHullShape*& shape)
{
	shape = CreateConvexHullShape(mesh->rMeshReference->vertices, mesh->rMeshReference->indices);

	collidersList.push_back(shape);

	btTransform startTransform;
	startTransform.setIdentity();

	btVector3 localInertia(0, 0, 0);

	if (mass != 0.f)
		shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.push_back(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);

	btRigidBody* body = new btRigidBody(rbInfo);
	PhysBody* pbody = new PhysBody(body);

	body->setUserPointer(pbody);
	world->addRigidBody(body);
	bodiesList.push_back(pbody);

	return pbody;
}

void ModulePhysics::RemoveBody(PhysBody* b)
{
	world->removeRigidBody(b->body);

	bodiesList.erase(std::find(bodiesList.begin(), bodiesList.end(), b));
	bodiesList.shrink_to_fit();
}

void ModulePhysics::RecalculateInertia(PhysBody* pbody, float mass, bool gravity)
{
	if (pbody && pbody->body)
	{
		btCollisionShape* colShape = pbody->body->getCollisionShape();
		btVector3 localInertia(0, 0, 0);

		if (!gravity)
			pbody->body->setGravity(btVector3(0, 0, 0));
		else
			pbody->body->setGravity(GRAVITY);

		if (mass != 0.f)
			colShape->calculateLocalInertia(mass, localInertia);

		pbody->body->setMassProps(mass, localInertia);
	}
}

// GETTERS ---------------------------------------------------------------
btVector3 ModulePhysics::GetWorldGravity()
{
	return world->getGravity();
}

bool ModulePhysics::GetDebugDraw()
{
	return debug;
}

Color ModulePhysics::GetColliderColor()
{
	return colliderColor;
}

// SETTERS ---------------------------------------------------------------
void ModulePhysics::SetWorldGravity(btVector3 g)
{
	world->setGravity(g);
}

void ModulePhysics::SetdebugDraw(bool d)
{
	debug = d;

	if (debug) LOG("DebugDrawer On");
	else LOG("DebugDrawer Off");
}

void ModulePhysics::SetColliderColor(Color col) 
{
	colliderColor = col;
}

void ModulePhysics::ResetGravity()
{
	LOG("Reseted Gravity");
	world->setGravity(GRAVITY);
}

// CONVEX HULL SHAPE ===============================================================
btConvexHullShape* ModulePhysics::CreateConvexHullShape(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
{
	btConvexHullShape* convexShape = new btConvexHullShape();

	// Add vertices to shape
	for (const auto& vertex : vertices) {
		btVector3 btVertex(vertex.position.x, vertex.position.y, vertex.position.z);
		convexShape->addPoint(btVertex);

		//LOG("Vertex: (%f, %f, %f)", vertex.position.x, vertex.position.y, vertex.position.z);
	}

	// Optionally, you can also add indices to maintain convexity
	//for (size_t i = 0; i < indices.size(); i += 3) {
	//	btVector3 vertex0(vertices[indices[i]].position.x, vertices[indices[i]].position.y, vertices[indices[i]].position.z);
	//	btVector3 vertex1(vertices[indices[i + 1]].position.x, vertices[indices[i + 1]].position.y, vertices[indices[i + 1]].position.z);
	//	btVector3 vertex2(vertices[indices[i + 2]].position.x, vertices[indices[i + 2]].position.y, vertices[indices[i + 2]].position.z);

	//	LOG("Vertex: (% .0f, % .0f, % .0f) - Index: (%d, %d, %d)", i, vertex0, vertex1, vertex2, i, indices[i], indices[i + 1], indices[i + 2]);

	//	convexShape->addPoint(vertex0);
	//	convexShape->addPoint(vertex1);
	//	convexShape->addPoint(vertex2);
	//}

	return convexShape;
}


// RayCasts ========================================================================
bool ModulePhysics::RayCast(const btVector3& from, const btVector3& to, btVector3& hitPoint)
{
	btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);

	// Realizar el raycast
	world->rayTest(from, to, rayCallback);

	// Comprobar si hubo una colisión
	if (rayCallback.hasHit()) {
		hitPoint = rayCallback.m_hitPointWorld; // Punto de impacto
		return true;
	}

	return false;
}

// DEBUG DRAWER =============================================
void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	line.origin.Set(from.getX(), from.getY(), from.getZ());
	line.destination.Set(to.getX(), to.getY(), to.getZ());
	line.color.Set(color.getX(), color.getY(), color.getZ());

	line.Render(External->physics->GetColliderColor()); // Custom color yey
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

btScalar* ModulePhysics::getOpenGLMatrix(float4x4 matrix)
{
	btScalar openGLMatrix[16];

	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			openGLMatrix[row * 4 + col] = btScalar(matrix[col][row]);
		}
	}
	return openGLMatrix;
}