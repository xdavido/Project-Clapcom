#pragma once

#include <vector>
#include "p2List.h"

#include "External/MathGeoLib/include/Math/float3.h"
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

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPosition(float3 pos);
	void SetRotation(Quat q);
	GameObject* SetGameObject(GameObject* _owner);
	void SetAsSensor(bool is_sensor);

	btRigidBody* body = nullptr;
	p2List<Module*> collision_listeners;

public:
	bool is_sensor = false;

	GameObject* owner;

	bool drawShape = true;
};