#include "CRigidBody.h"

#include "External/ImGui/imgui.h"
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
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	if (ImGui::CollapsingHeader("RigidBody", flags))
	{

	}
}

btRigidBody* CRigidBody::GetBody()
{
	return body;
}