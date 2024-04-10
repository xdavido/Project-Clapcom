#pragma once

#include <vector>
#include "p2List.h"

#include "External/MathGeoLib/include/Math/float3.h"
#include "External/MathGeoLib/include/Math/float4x4.h"
#include "External/MathGeoLib/include/Math/Quat.h"

class btRigidBody;
class Module;
class GameObject;

struct PhysBody
{
	friend class ModulePhysics;

public:

	PhysBody(btRigidBody* body);
	~PhysBody();

	void GetTransform(float4x4& matrix) const;
	void SetTransform(const float4x4& matrix) const;

	void SetPosition(const float3& pos) const;
	void SetRotation(const Quat& q) const;

	void Push(const float3& pushVec) const;

	void SetGameObject(GameObject* owner);
	

	btRigidBody* body;
	p2List<Module*> collision_listeners;

public:

	GameObject* owner;

	bool isSensor;
	bool drawShape;

};