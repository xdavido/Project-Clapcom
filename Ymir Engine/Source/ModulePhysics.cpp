#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "PhysBody.h"
#include "GameObject.h"
#include "CScript.h"

#include "ModuleScene.h"

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
	debugScene = true;
	debugGame = false;

	// Colors
	colliderColor = Green;
	sensorColor = Red;

	CreateWorld();
	world->setGravity(GRAVITY);

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
	float fixedTimeStep = 1 / App->GetFPS();

	if (TimeManager::gameTimer.GetState() == TimerState::RUNNING) 
	{
		world->stepSimulation(dt, 1, fixedTimeStep);
	}
	else
	{
		world->stepSimulation(0);
	}

	return UPDATE_CONTINUE;
}

// UPDATE --------------------------------------------------------------------
update_status ModulePhysics::Update(float dt)
{
	//LOG("Bodies in list: %d", bodiesList.size());
	//LOG("Bodies in world: %d", world->getNumCollisionObjects());

	return UPDATE_CONTINUE;
}

// POST-UPDATE ---------------------------------------------------------------
update_status ModulePhysics::PostUpdate(float dt)
{
	//for (auto it = bodiesList.begin(); it != bodiesList.end(); ++it)
	//{
	//	btRigidBody* b = (btRigidBody*)(*it)->body;
	//	btTransform t;
	//	b->getMotionState()->getWorldTransform(t);

	//	//LOG("Pos: %f, %f, %f", t.getOrigin().x, t.getOrigin().y, t.getOrigin().z );
	//}

	if (TimeManager::gameTimer.GetState() == TimerState::RUNNING)
	{	
		// Enable/disable collision logic in God Mode (WIP, doesn't work properly now)
		//if (App->scene->godMode)
		//{
		//	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		//	{
		//		for (auto it = bodiesList.begin(); it != bodiesList.end(); ++it)
		//		{
		//			if ((*it)->body != NULL)
		//			{
		//				if ((*it)->body->getCollisionFlags() == btCollisionObject::CF_NO_CONTACT_RESPONSE)
		//				{
		//					(*it)->body->setCollisionFlags((*it)->body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);

		//				}
		//				else
		//				{
		//					(*it)->body->setCollisionFlags((*it)->body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

		//				}
		//			}
		//		}
		//	}
		//}
		
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
				if (pbodyA->owner != nullptr && pbodyB->owner != nullptr) {
					CScript* auxA = dynamic_cast<CScript*>(pbodyA->owner->GetComponent(ComponentType::SCRIPT));
					CScript* auxB = dynamic_cast<CScript*>(pbodyB->owner->GetComponent(ComponentType::SCRIPT));

					if (auxA != nullptr && auxB != nullptr) {
						if (firstCollision) {
							auxA->CollisionEnterCallback(false, pbodyB->owner);
							auxB->CollisionEnterCallback(false, pbodyA->owner);
							firstCollision = false;
							inCollision = true;
						}
						else if (inCollision) {
							auxA->CollisionStayCallback(false, pbodyB->owner);
							auxB->CollisionStayCallback(false, pbodyA->owner);
							onExitCollision = true;
						}
					}
				}

				if (pbodyA && pbodyB)
				{
					p2List_item<Module*>* item = pbodyA->collision_listeners.getFirst();
					while (item)
					{
						item->data->OnCollision(pbodyA, pbodyB);
						item = item->next;
					}

					item = pbodyB->collision_listeners.getFirst();
					while (item)
					{
						item->data->OnCollision(pbodyB, pbodyA);
						item = item->next;
					}
				}
			}
			else if (numContacts == 0 && onExitCollision)
			{
				PhysBody* pbodyA = (PhysBody*)obA->getUserPointer();
				PhysBody* pbodyB = (PhysBody*)obB->getUserPointer();

				if (pbodyA && pbodyB)
				{
					CScript* scriptA = dynamic_cast<CScript*>(pbodyA->owner->GetComponent(ComponentType::SCRIPT));
					CScript* scriptB = dynamic_cast<CScript*>(pbodyB->owner->GetComponent(ComponentType::SCRIPT));

					if (scriptA && scriptB)
					{
						scriptA->CollisionExitCallback(false, pbodyB->owner);
						scriptB->CollisionExitCallback(false, pbodyA->owner);
					}

					
				}
			}
		
		}
		


	}

	return UPDATE_CONTINUE;
}

// CLEANUP -------------------------------------------------------------------
bool ModulePhysics::CleanUp()
{



	return true;
}

// CREATE / DELETE WORLD --------------------------------------------------------------
void ModulePhysics::CreateWorld()
{
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, constraintSolver, collisionConfig);
}

void ModulePhysics::DeleteWorld()
{
	External->physics->ClearBodiesList();
	External->physics->motions.clear();
	External->physics->world->clearForces();

	delete world;
}

// ADDBODY ============================================================================================================
// Box Collider -------------------------------------------------------------------------------------------------------
PhysBody* ModulePhysics::AddBody(CCube cube, PhysicsType physType, float mass, bool useGravity, btCollisionShape*& shape)
{
	shape = new btBoxShape(btVector3(cube.size.x * 0.5f, cube.size.y * 0.5f, cube.size.z * 0.5f));

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(cube.transform.ptr());
	
	//float x = startTransform.getOrigin().x();
	//LOG("start transform: %f", x);
	//float y = startTransform.getOrigin().y();
	//LOG("start transform: %f", y);
	//startTransform.setOrigin(btVector3(x, y, 0));
	//float z = startTransform.getOrigin().z();
	//LOG("start transform: %f", z);

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
PhysBody* ModulePhysics::AddBody(CSphere sphere, PhysicsType physType, float mass, bool useGravity, btCollisionShape*& shape)
{
	shape = new btSphereShape(sphere.radius);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(sphere.transform.ptr());

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
PhysBody* ModulePhysics::AddBody(CCapsule capsule, PhysicsType physType, float mass, bool useGravity, btCollisionShape*& shape)
{
	shape = new btCapsuleShape(capsule.height, capsule.radius);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(capsule.transform.ptr());

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
PhysBody* ModulePhysics::AddBody(CCone cone, PhysicsType physType, float mass, bool useGravity, btCollisionShape*& shape)
{
	shape = new btConeShape(cone.height, cone.radius);

	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(cone.transform.ptr());

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

// Cylinder --------------------------------------------------------------------------------------------------------------
PhysBody* ModulePhysics::AddBody(CCylinder cylinder, PhysicsType physType, float mass, bool useGravity, btCollisionShape*& shape)
{
	btVector3 vec = btVector3(cylinder.radius * 2, cylinder.height, cylinder.radius * 2);
	shape = new btCylinderShape(vec);
	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(cylinder.transform.ptr());

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
PhysBody* ModulePhysics::AddBody(CMesh* mesh, PhysicsType, float mass, bool useGravity, btCollisionShape*& shape)
{
	shape = CreateCollisionShape(mesh->rMeshReference->vertices, mesh->rMeshReference->indices);

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

// Destroy things (yey)
void ModulePhysics::RemoveBody(PhysBody* b)
{
	if (b->body != nullptr)
		world->removeCollisionObject(b->body);

	bodiesList.erase(std::find(bodiesList.begin(), bodiesList.end(), b));
	bodiesList.shrink_to_fit();
}

void ModulePhysics::ClearBodiesList()
{
	for (int i = 0; i < bodiesList.size(); i++)
	{
		RemoveBody(bodiesList[i]);
	}

	bodiesList.clear();
}

void ModulePhysics::RecalculateInertia(PhysBody* pbody, float mass, bool useGravity)
{
	if (pbody && pbody->body)
	{
		btCollisionShape* colShape = pbody->body->getCollisionShape();
		btVector3 localInertia(0, 0, 0);

		if (!useGravity)
			pbody->body->setGravity(btVector3(0, 0, 0));
		else
			pbody->body->setGravity(world->getGravity());

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

Color ModulePhysics::GetColliderColor()
{
	return colliderColor;
}

// SETTERS ---------------------------------------------------------------
void ModulePhysics::SetWorldGravity(btVector3 g)
{
	world->setGravity(g);
}

void ModulePhysics::SetDrawScene(bool d)
{
	debugScene = d;

	if (debugScene) LOG("Draw Scene Colliders On");
	else LOG("Draw Scene Colliders Off");
}

void ModulePhysics::SetDrawGame(bool d)
{
	debugGame = d;

	if (debugScene) LOG("Draw Game Colliders On");
	else LOG("Draw Game Colliders Off");
}

void ModulePhysics::SetColliderColor(Color col)
{
	colliderColor = col;
}

void ModulePhysics::SetSensorColor(Color col)
{
	sensorColor = col;
}

void ModulePhysics::SetLineWidth(float w)
{
	shapeLineWidth = w;
}

bool ModulePhysics::GetDebugDraw()
{
	return true;
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

// RENDER SHAPES ---------------------------------------------------------------
void ModulePhysics::RenderBoxCollider(PhysBody* pbody)
{
	float4x4 mat;
	pbody->GetTransform(mat);

	btBoxShape* shape = (btBoxShape*)pbody->body->getCollisionShape();
	btVector3 halfExtents = shape->getHalfExtentsWithoutMargin();

	glPushMatrix();
	glMultMatrixf(mat.ptr()); // translation and rotation

	// TODO: render box
	glBegin(GL_LINES);

	// Aristas horizontales
	glVertex3f(-halfExtents.x(), -halfExtents.y(), -halfExtents.z());
	glVertex3f(halfExtents.x(), -halfExtents.y(), -halfExtents.z());

	glVertex3f(-halfExtents.x(), halfExtents.y(), -halfExtents.z());
	glVertex3f(halfExtents.x(), halfExtents.y(), -halfExtents.z());

	glVertex3f(-halfExtents.x(), halfExtents.y(), halfExtents.z());
	glVertex3f(halfExtents.x(), halfExtents.y(), halfExtents.z());

	glVertex3f(-halfExtents.x(), -halfExtents.y(), halfExtents.z());
	glVertex3f(halfExtents.x(), -halfExtents.y(), halfExtents.z());

	// Aristas verticales
	glVertex3f(-halfExtents.x(), -halfExtents.y(), -halfExtents.z());
	glVertex3f(-halfExtents.x(), halfExtents.y(), -halfExtents.z());

	glVertex3f(halfExtents.x(), -halfExtents.y(), -halfExtents.z());
	glVertex3f(halfExtents.x(), halfExtents.y(), -halfExtents.z());

	glVertex3f(halfExtents.x(), -halfExtents.y(), halfExtents.z());
	glVertex3f(halfExtents.x(), halfExtents.y(), halfExtents.z());

	glVertex3f(-halfExtents.x(), -halfExtents.y(), halfExtents.z());
	glVertex3f(-halfExtents.x(), halfExtents.y(), halfExtents.z());

	// Otras aristas
	glVertex3f(-halfExtents.x(), -halfExtents.y(), -halfExtents.z());
	glVertex3f(-halfExtents.x(), -halfExtents.y(), halfExtents.z());

	glVertex3f(halfExtents.x(), -halfExtents.y(), -halfExtents.z());
	glVertex3f(halfExtents.x(), -halfExtents.y(), halfExtents.z());

	glVertex3f(-halfExtents.x(), halfExtents.y(), -halfExtents.z());
	glVertex3f(-halfExtents.x(), halfExtents.y(), halfExtents.z());

	glVertex3f(halfExtents.x(), halfExtents.y(), -halfExtents.z());
	glVertex3f(halfExtents.x(), halfExtents.y(), halfExtents.z());

	glEnd();

	glPopMatrix();
}
void ModulePhysics::RenderSphereCollider(PhysBody* pbody)
{
	float4x4 mat;
	pbody->GetTransform(mat);

	float r = ((btSphereShape*)pbody->body->getCollisionShape())->getRadius();

	glPushMatrix();
	glMultMatrixf(mat.ptr()); // translation and rotation

	// Dibujar la meridiana en el plano XY
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 360; i += 10) {
		float phi = i * DEGTORAD;
		glVertex3f(r * cos(phi), r * sin(phi), 0);
	}
	glEnd();

	// Dibujar la meridiana en el plano XZ
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 360; i += 10) {
		float phi = i * DEGTORAD;
		glVertex3f(r * cos(phi), 0, r * sin(phi));
	}
	glEnd();

	// Dibujar la meridiana en el plano YZ
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 360; i += 10) {
		float phi = i * DEGTORAD;
		glVertex3f(0, r * cos(phi), r * sin(phi));
	}
	glEnd();

	glPopMatrix();
}
void ModulePhysics::RenderCapsuleCollider(PhysBody* pbody)
{
	float4x4 mat;
	pbody->GetTransform(mat);

	float radius = ((btCapsuleShape*)pbody->body->getCollisionShape())->getRadius();
	float halfHeight = ((btCapsuleShape*)pbody->body->getCollisionShape())->getHalfHeight();

	glPushMatrix();
	glMultMatrixf(mat.ptr()); // translation and rotation

	// Columnas
	glBegin(GL_LINES);

	glVertex3f(radius, halfHeight, 0);
	glVertex3f(radius, -halfHeight, 0);

	glVertex3f(-radius, halfHeight, 0);
	glVertex3f(-radius, -halfHeight, 0);

	glVertex3f(0, halfHeight, radius);
	glVertex3f(0, -halfHeight, radius);

	glVertex3f(0, halfHeight, -radius);
	glVertex3f(0, -halfHeight, -radius);

	glEnd();

	// Dibujar la meridiana en el plano XZ (circulos completos)
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 360; i += 10) {
		float phi = i * DEGTORAD;
		glVertex3f(radius * cos(phi), -halfHeight, radius * sin(phi));
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 360; i += 10) {
		float phi = i * DEGTORAD;
		glVertex3f(radius * cos(phi), halfHeight, radius * sin(phi));
	}
	glEnd();

	// Medio circulo superior (XY)
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 180; i += 10) {
		float phi = i * DEGTORAD;
		glVertex3f(radius * cos(phi), (radius * sin(phi)) + halfHeight, 0);
	}
	glEnd();

	// Medio circulo inferior (XY)
	glBegin(GL_LINE_STRIP);
	for (int i = 180; i <= 360; i += 10) {
		float phi = i * DEGTORAD;
		glVertex3f(radius * cos(phi), (radius * sin(phi)) - halfHeight, 0);
	}
	glEnd();

	// Medio circulo superior (ZY)
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 180; i += 10) {
		float phi = i * DEGTORAD;
		glVertex3f(0, (radius * sin(phi)) + halfHeight, radius * cos(phi));
	}
	glEnd();

	// Medio circulo inferior (ZY)
	glBegin(GL_LINE_STRIP);
	for (int i = 180; i <= 360; i += 10) {
		float phi = i * DEGTORAD;
		glVertex3f(0, (radius * sin(phi)) - halfHeight, radius * cos(phi));
	}
	glEnd();


	glPopMatrix();
}

void ModulePhysics::RenderConeCollider(PhysBody* pbody)
{
	float4x4 mat;
	pbody->GetTransform(mat);

	float radius = ((btConeShape*)pbody->body->getCollisionShape())->getRadius();
	float halfHeight = ((btConeShape*)pbody->body->getCollisionShape())->getHeight() / 2;

	glPushMatrix();
	glMultMatrixf(mat.ptr()); // translation and rotation

	// Columnas
	glBegin(GL_LINES);

	glVertex3f(0, halfHeight, 0);
	glVertex3f(radius, -halfHeight, 0);

	glVertex3f(0, halfHeight, 0);
	glVertex3f(-radius, -halfHeight, 0);

	glVertex3f(0, halfHeight, 0);
	glVertex3f(0, -halfHeight, radius);

	glVertex3f(0, halfHeight, 0);
	glVertex3f(0, -halfHeight, -radius);

	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 360; i += 10) {
		float phi = i * DEGTORAD;
		glVertex3f(radius * cos(phi), -halfHeight, radius * sin(phi));
	}
	glEnd();

	glPopMatrix();
}

void ModulePhysics::RenderCylinderCollider(PhysBody* pbody)
{
	float4x4 mat;
	pbody->GetTransform(mat);

	float radius = ((btCapsuleShape*)pbody->body->getCollisionShape())->getRadius();
	float halfHeight = ((btCapsuleShape*)pbody->body->getCollisionShape())->getHalfHeight();

	glPushMatrix();
	glMultMatrixf(mat.ptr()); // translation and rotation

	// Columnas
	glBegin(GL_LINES);

	glVertex3f(radius, halfHeight, 0);
	glVertex3f(radius, -halfHeight, 0);

	glVertex3f(-radius, halfHeight, 0);
	glVertex3f(-radius, -halfHeight, 0);

	glVertex3f(0, halfHeight, radius);
	glVertex3f(0, -halfHeight, radius);

	glVertex3f(0, halfHeight, -radius);
	glVertex3f(0, -halfHeight, -radius);

	glEnd();

	// Dibujar la meridiana en el plano XZ (circulos completos)
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 360; i += 10) {
		float phi = i * DEGTORAD;
		glVertex3f(radius * cos(phi), -halfHeight, radius * sin(phi));
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 360; i += 10) {
		float phi = i * DEGTORAD;
		glVertex3f(radius * cos(phi), halfHeight, radius * sin(phi));
	}
	glEnd();

	glPopMatrix();
}

void ModulePhysics::RenderMeshCollider(PhysBody* pbody)
{
	float4x4 mat;
	pbody->GetTransform(mat);

	// Get Scale
	btVector3 scaling = pbody->body->getCollisionShape()->getLocalScaling();

	btConvexTriangleMeshShape* shape = (btConvexTriangleMeshShape*)pbody->body->getCollisionShape();
	btStridingMeshInterface* meshInterface = shape->getMeshInterface();

	int numTriangles = meshInterface->getNumSubParts();

	// Render mesh 
	glPushMatrix(); 
	glMultMatrixf(mat.ptr()); // translation and rotation  

	for (int part = 0; part < numTriangles; ++part) {
		const unsigned char* vertexBase;
		int numVerts, vertexStride;
		const unsigned char* indexBase;
		int indexStride, numFaces;
		PHY_ScalarType indexType, vertexType;

		meshInterface->getLockedReadOnlyVertexIndexBase(&vertexBase, numVerts, vertexType, vertexStride, &indexBase, indexStride, numFaces, indexType, part);

		glBegin(GL_LINES);
		for (int f = 0; f < numFaces; ++f) {
			unsigned int* index = (unsigned int*)(indexBase + f * indexStride);

			for (int i = 0; i < 3; ++i) {
				btVector3* vertex1 = (btVector3*)(vertexBase + index[i] * vertexStride);
				btVector3* vertex2 = (btVector3*)(vertexBase + index[(i + 1) % 3] * vertexStride);

				// Apply scale
				btVector3 scaledVertex1 = (*vertex1) * scaling;
				btVector3 scaledVertex2 = (*vertex2) * scaling;

				glVertex3f(scaledVertex1.x(), scaledVertex1.y(), scaledVertex1.z());
				glVertex3f(scaledVertex2.x(), scaledVertex2.y(), scaledVertex2.z());
			}
		}

		glEnd();

		meshInterface->unLockReadOnlyVertexBase(part);
	}

	glPopMatrix();
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