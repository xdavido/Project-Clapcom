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
	CAPSULE,
	CONE,
	CYLINDER,
	MESH_COLLIDER,
};

enum PhysicsType {
	DYNAMIC,
	KINEMATIC,
	STATIC
};

class CCollider : public Component
{
public:
	CCollider(GameObject* owner, ColliderType collider = ColliderType::BOX, PhysicsType physics = PhysicsType::DYNAMIC);
	~CCollider();

	void Update();
	void OnInspector();

	btCollisionShape* GetShape();

	void UpdateLockRotation();

	//TODO: funcions per canviar transform del collider, isTrigger, hull collider, mesh collider, etc.
	void SetBoxCollider();
	void SetSphereCollider();
	void SetCapsuleCollider();
	void SetConeCollider();
	void SetCylinderCollider();
	void SetMeshCollider();

	void AdaptSizeToShape(ColliderType prevType);
	void SetDefaultValues(PhysicsType type);
	void SetOBBSize();
	void SetAsSensor(bool is_sensor);

	void RemovePhysbody();

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
	
	ColliderType collType;
	PhysicsType physType;

	float3 size;
	btVector3 btSize;
	float3 offset;

	float mass;
	float friction;
	float angularFriction;
	bool useGravity;
	bool isSensor;

	bool lockX, lockY, lockZ;

	btCollisionShape* shape;
	//btConvexHullShape* convexShape;

public:

	PhysBody* physBody;

	CTransform* transform;
	float4x4 trans;
	//btCollisionObject* collider;

	//TODO: crear variables: hull, mesh, sensor, edit collider, transform�?

};