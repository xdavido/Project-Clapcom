#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

#include "External/Bullet/include/btBulletDynamicsCommon.h"
#include "External/glmath.h"

#include <vector>

class DebugDrawer;

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

	// TODO: LLUC i MARC
	void AddBody(btRigidBody* b);
	void AddCollider(btCollisionShape* c);

	void RemoveBody(btRigidBody* b);
	void RemoveCollider(btCollisionShape* c);

	// TODO: NIXON
	// RayCast functions

public:
	// TODO: create 2 lists, one for rigidbodies and one for colliders	
	std::vector<btRigidBody*> bodiesList;
	std::vector<btCollisionShape*> collidersList;

private:

	bool debug = true; // If true, draws colliders
	DebugDrawer* debugDraw;

	btDynamicsWorld* world; // World se puede dejar en public si se requiere
	btDispatcher* dispatcher;
	btCollisionConfiguration* collisionConfig;
	btBroadphaseInterface* broadphase;
	btConstraintSolver* constraintSolver;

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