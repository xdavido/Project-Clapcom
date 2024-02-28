#include "CRigidBody.h"

#include "External/Bullet/include/btBulletDynamicsCommon.h"

CRigidBody::CRigidBody(GameObject* owner) : Component(owner, ComponentType::RIGIDBODY)
{
	this->mOwner = owner;
}

CRigidBody::~CRigidBody()
{
	delete body;
}

void CRigidBody::Update()
{

}

void CRigidBody::OnInspector()
{

}