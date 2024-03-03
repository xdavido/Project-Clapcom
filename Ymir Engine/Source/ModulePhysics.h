#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "Color.h"
#include "CMesh.h"

#include "External/Bullet/include/btBulletDynamicsCommon.h"
//#include "External/glmath.h"

#include <vector>

class DebugDrawer;
struct PhysBody;
enum physicsType;
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

	//Getters
	btVector3 GetWorldGravity();
	bool GetDebugDraw();
	Color GetColliderColor();

	//Setters
	void SetWorldGravity(btVector3 g);
	void SetdebugDraw(bool d);
	void SetColliderColor(Color col);

	void ResetGravity(); // Sets the world gravity to GRAVITY from globals.h

	// TODO: LLUC i MARC
	// PhysBody related
	PhysBody* AddBody(CCube cube, physicsType physType, float mass, bool gravity, btCollisionShape*& shape);		 // Box
	PhysBody* AddBody(CSphere sphere, physicsType physType, float mass, bool gravity, btCollisionShape*& shape);	 // Sphere
	PhysBody* AddBody(CCapsule capsule, physicsType physType, float mass, bool gravity, btCollisionShape*& shape); // Capsule
	PhysBody* AddBody(CMesh* mesh, physicsType, float mass, bool gravity, btConvexHullShape*& shape);				 // Convex
	void RemoveBody(PhysBody* b);

	void RecalculateInertia(PhysBody* pbody, float mass, bool gravity);

	// Collision Shape
	btConvexHullShape* CreateConvexHullShape(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);

	// TODO: NIXON
	// RayCast functions
	bool RayCast(const btVector3& from, const btVector3& to, btVector3& hitPoint);

	btScalar* getOpenGLMatrix(float4x4 matrix);

public:
	// TODO: create 2 lists, one for rigidbodies and one for colliders	
	std::vector<btCollisionShape*> collidersList;
	std::vector<PhysBody*> bodiesList;
	std::vector <btDefaultMotionState*> motions;

	btDiscreteDynamicsWorld* world; // World se puede dejar en public si se requiere

	bool debug = true; // If true, draws colliders

	DebugDrawer* debugDraw;

	btDispatcher* dispatcher;
	btCollisionConfiguration* collisionConfig;
	btBroadphaseInterface* broadphase;
	btConstraintSolver* constraintSolver;

private:
	Color colliderColor;
};

class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer() : line(0, 0, 0)
	{}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;
	CLine line;
	Primitive point;
};