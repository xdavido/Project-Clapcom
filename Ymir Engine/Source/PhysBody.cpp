#include "PhysBody.h"
#include "External/Bullet/include/btBulletDynamicsCommon.h"

#include "External/mmgr/mmgr.h"

// =================================================
PhysBody::PhysBody(btRigidBody* body) : body(body)
{}

// ---------------------------------------------------------
PhysBody::~PhysBody()
{
	//delete body;
}

// ---------------------------------------------------------
void PhysBody::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

// ---------------------------------------------------------
void PhysBody::GetTransform(float* matrix) const
{
	if (body != NULL && matrix != NULL)
	{
		body->getWorldTransform().getOpenGLMatrix(matrix);
	}
}

// ---------------------------------------------------------
void PhysBody::SetTransform(const float* matrix) const
{
	if (body != NULL && matrix != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

// ---------------------------------------------------------
void PhysBody::SetPosition(float3 pos)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(pos.x, pos.y, pos.z));
	
	body->setWorldTransform(t);
}

// Ángulos en radianes!!!
void PhysBody::SetRotation(Quat q)
{
	btTransform t = body->getWorldTransform();

	btQuaternion rotationQuat;
	rotationQuat.setW(q.w);
	rotationQuat.setX(q.x);
	rotationQuat.setY(q.y);
	rotationQuat.setZ(q.z);

	t.setRotation(rotationQuat);
	body->setWorldTransform(t);
}

// ---------------------------------------------------------
void PhysBody::SetAsSensor(bool is_sensor)
{
	if (this->is_sensor != is_sensor)
	{
		this->is_sensor = is_sensor;
		if (is_sensor == true)
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		else
			body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}
