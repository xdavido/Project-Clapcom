#pragma once
#include "Application.h"
#include "Module.h"
#include "ModulePhysics.h"
#include "Primitive.h"
#include "CTransform.h"
#include "PhysBody.h"

#include "Component.h"
#include <vector>

class btCollisionShape;
class btCollisionObject;
class btRigidBody;

enum ColliderType 
{
	BOX,
	SPHERE,
	CYLINDER,
	CONVEX_HULL,
	MESH_COLLIDER
};

enum physicsType {
	DYNAMIC,
	KINEMATIC,
	STATIC
};

class CCollider : public Component
{
public:
	CCollider(GameObject* owner);
	~CCollider();

	void Update();
	void OnInspector();

	btCollisionShape* GetShape();

	//TODO: funcions per canviar transform del collider, isTrigger, hull collider, mesh collider, etc.
	void SetBoxCollider();
	void SetSphereCollider();
	void SetCylinderCollider();
	void SetConvexCollider();
	void SetMeshCollider();

	void RemovePhysbody();

	void SetDefaultValues(physicsType type);

	// Conversion function from btVector3 to float3
	float3 btVector3_to_float3(const btVector3& v) {
		return { v.x(), v.y(), v.z() };
	}

	// Conversion function from float3 to btVector3
	btVector3 float3_to_btVector3(const float3& v) {
		return btVector3(v.x, v.y, v.z);
	}

public:

	// Public para el Save/Load
	float3 size;
	btVector3 btSize;

	float mass;
	btCollisionShape* shape;
	btConvexHullShape* convexShape;

private:

	ColliderType collType;
	PhysBody* physBody;

	CTransform* transform;

	//btCollisionObject* collider;

	//TODO: crear variables: hull, mesh, sensor, edit collider, transform¿?

	physicsType physType;
	bool gravity;
};