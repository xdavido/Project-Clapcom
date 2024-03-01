#pragma once

#include <vector>

class btRigidBody;
class Module;


struct PhysBody
{
	friend class ModulePhysics;
public:
	PhysBody(btRigidBody* body);
	~PhysBody();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void SetRot(float x, float y, float z);

	void SetAsSensor(bool is_sensor);
	btRigidBody* body = nullptr;

public:
	bool is_sensor = false;
};