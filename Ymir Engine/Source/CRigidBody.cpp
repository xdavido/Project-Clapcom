#include "CRigidBody.h"
#include "GameObject.h"

#include "External/ImGui/imgui.h"
#include "External/Bullet/include/btBulletDynamicsCommon.h"

//CRigidBody::CRigidBody(GameObject* owner) : Component(owner, ComponentType::RIGIDBODY)
//{
//	this->mOwner = owner;
//
//	physType = physicsType::DYNAMIC;
//	mass = 1;
//}
//
//CRigidBody::~CRigidBody()
//{
//	delete body;
//}
//
//void CRigidBody::Update()
//{
//
//}
//
//void CRigidBody::OnInspector()
//{
//	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
//
//	if (ImGui::CollapsingHeader("RigidBody", flags))
//	{
//		ImGui::PushItemWidth(120.0f);
//
//		const char* items[] = { "Dynamic", "Kinematic", "Static" };
//		int currentItem = static_cast<int>(physType);
//
//		ImGui::Combo("Physics Type\t", &currentItem, items, IM_ARRAYSIZE(items));
//		physType = static_cast<physicsType>(currentItem);
//
//		if (physType != physicsType::STATIC)
//			ImGui::DragFloat("Mass\t", &mass);
//		if (physType == physicsType::DYNAMIC)
//			ImGui::Checkbox("Use gravity\t", &gravity);
//	}
//}
//
//btRigidBody* CRigidBody::GetBody()
//{
//	return body;
//}