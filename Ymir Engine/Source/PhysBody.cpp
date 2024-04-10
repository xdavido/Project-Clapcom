#include "PhysBody.h"
#include "External/Bullet/include/btBulletDynamicsCommon.h"

#include "External/mmgr/mmgr.h"

// =================================================
PhysBody::PhysBody(btRigidBody* body) : body(body), owner(nullptr), isSensor(false), drawShape(true)
{

}

// ---------------------------------------------------------
PhysBody::~PhysBody()
{
	RELEASE(body);
}

// ---------------------------------------------------------
void PhysBody::Push(const float3& pushVec) const
{
	body->applyCentralImpulse(btVector3(pushVec.x, pushVec.y, pushVec.z));
}

// ---------------------------------------------------------
void PhysBody::GetTransform(float4x4& matrix) const
{
	if (body != nullptr)
	{
		body->getWorldTransform().getOpenGLMatrix(matrix.ptr());
	}
}

// ---------------------------------------------------------
void PhysBody::SetTransform(const float4x4& matrix) const
{
	if (body != nullptr && matrix.ptr() != nullptr)
	{
		btTransform t;

		t.setFromOpenGLMatrix(matrix.ptr());

		body->setWorldTransform(t);
	}
}

// ---------------------------------------------------------
void PhysBody::SetPosition(const float3& pos) const
{
	btTransform t = body->getWorldTransform();

	t.setOrigin(btVector3(pos.x, pos.y, pos.z));
	
	body->setWorldTransform(t);
}

// �ngulos en radianes!!!
void PhysBody::SetRotation(const Quat& q) const
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

void PhysBody::SetGameObject(GameObject* owner)
{
	this->owner = owner;
}