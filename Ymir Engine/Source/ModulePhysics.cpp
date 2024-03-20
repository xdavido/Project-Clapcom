#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "PhysBody.h"
#include "GameObject.h"
#include "CScript.h"

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

		int numManifolds = world->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; i++)
		{
			btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
			btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
			btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

			int numContacts = contactManifold->getNumContacts();
			if (numContacts > 0)
			{
				//LOG("Contacts number: %i", contactManifold->getNumContacts());
				PhysBody* pbodyA = (PhysBody*)obA->getUserPointer();
				PhysBody* pbodyB = (PhysBody*)obB->getUserPointer();

				// Pendiente de revisar de momento wuaaaaarrrraaaaaaaaaada!!!!
				if (pbodyA->owner != nullptr) {
					CScript* aux = dynamic_cast<CScript*>(pbodyA->owner->GetComponent(ComponentType::SCRIPT));

					if (aux != nullptr) {
						aux->CollisionCallback(false, pbodyB->owner);
					}
				}



				if (pbodyB->owner != nullptr) {
					CScript* aux = dynamic_cast<CScript*>(pbodyB->owner->GetComponent(ComponentType::SCRIPT));

					if (aux != nullptr) {
						aux->CollisionCallback(false, pbodyA->owner);
					}
				}
				

				if (pbodyA && pbodyB)
				{
					p2List_item<Module*>* item = pbodyA->collision_listeners.getFirst();
					while (item)
					{
						item->data->OnCollision(pbodyA, pbodyB);
						item = item->next;

						if (pbodyA->owner != nullptr) {
							CScript* aux = dynamic_cast<CScript*>(pbodyA->owner->GetComponent(ComponentType::SCRIPT));

							if (aux != nullptr) {
								aux->CollisionCallback(false, pbodyA->owner);
							}
						}
					}

					item = pbodyB->collision_listeners.getFirst();
					while (item)
					{
						item->data->OnCollision(pbodyB, pbodyA);
						item = item->next;

						if (pbodyB->owner != nullptr) {
							CScript* aux = dynamic_cast<CScript*>(pbodyB->owner->GetComponent(ComponentType::SCRIPT));

							if (aux != nullptr) {
								aux->CollisionCallback(false, pbodyB->owner);
							}
						}
					}
				}
			}
		}
	}

	return UPDATE_CONTINUE;
}

// POST-UPDATE ---------------------------------------------------------------
update_status ModulePhysics::PostUpdate(float dt)
{
	beginPlay = false;
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
PhysBody* ModulePhysics::AddBody(CCube cube, PhysicsType physType, float mass, bool gravity, btCollisionShape*& shape)
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
PhysBody* ModulePhysics::AddBody(CSphere sphere, PhysicsType physType, float mass, bool gravity, btCollisionShape*& shape)
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
PhysBody* ModulePhysics::AddBody(CCapsule capsule, PhysicsType physType, float mass, bool gravity, btCollisionShape*& shape)
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

// Mesh Collider ----------------------------------------------------------------------------------------------------
PhysBody* ModulePhysics::AddBody(CMesh* mesh, PhysicsType, float mass, bool gravity, btCollisionShape*& shape)
{
	shape = CreateCollisionShape(mesh->rMeshReference->vertices, mesh->rMeshReference->indices);

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

// MESH SHAPE ===============================================================
btCollisionShape* ModulePhysics::CreateCollisionShape(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
{
	btTriangleMesh* triangleMesh = new btTriangleMesh();

	// Add vertices to the triangle mesh
    for (const Vertex& vertex : vertices) {
        btVector3 btVertex(vertex.position.x, vertex.position.y, vertex.position.z);
        triangleMesh->findOrAddVertex(btVertex, 1);
    }

	// Add triangles to the triangle mesh
	for (size_t i = 0; i < indices.size(); i += 3) {
		const Vertex& v0 = vertices[indices[i]];
		const Vertex& v1 = vertices[indices[i + 1]];
		const Vertex& v2 = vertices[indices[i + 2]];

		btVector3 btV0(v0.position.x, v0.position.y, v0.position.z);
		btVector3 btV1(v1.position.x, v1.position.y, v1.position.z);
		btVector3 btV2(v2.position.x, v2.position.y, v2.position.z);

		triangleMesh->addTriangle(btV0, btV1, btV2);
	}

	btCollisionShape* collisionShape = new btConvexTriangleMeshShape(triangleMesh);
	
	return collisionShape;
}

// RayCasts ========================================================================
bool ModulePhysics::RayCast(const btVector3& from, const btVector3& to, btVector3& hitPoint)
{
	btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);

	// Realizar el raycast
	world->rayTest(from, to, rayCallback);

	// Comprobar si hubo una colisi�n
	if (rayCallback.hasHit()) {
		hitPoint = rayCallback.m_hitPointWorld; // Punto de impacto
		return true;
	}

	return false;
}

bool ModulePhysics::VolumetricRayCast(const btVector3& origin, const btVector3& direction, int numRays, float rayLength, std::vector<btVector3>& hitPoints)
{

	btVector3 step = direction.normalized() * (rayLength / numRays);
	
	btVector3 start = origin - direction.normalized() * (rayLength / 2);

	hitPoints.clear();


	for (int i = 0; i < numRays; ++i) {
		btVector3 end = start + direction.normalized() * rayLength;

		btCollisionWorld::ClosestRayResultCallback rayCallback(start, end);

		world->rayTest(start, end, rayCallback);

		if (rayCallback.hasHit()) {
			hitPoints.push_back(rayCallback.m_hitPointWorld);
		}

		
		start += step;
	}

	return !hitPoints.empty();
}

bool ModulePhysics::DirectionalRayCast(const btVector3& origin, const btVector3& direction, float rayLength, btVector3& hitPoint) //the most common
{
	btVector3 end = origin + direction.normalized() * rayLength;

	btCollisionWorld::ClosestRayResultCallback rayCallback(origin, end);

	world->rayTest(origin, end, rayCallback);

	if (rayCallback.hasHit()) {
		hitPoint = rayCallback.m_hitPointWorld;
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

