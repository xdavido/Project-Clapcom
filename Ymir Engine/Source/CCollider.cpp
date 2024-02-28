#include "CCollider.h"
#include "GameObject.h"
#include "ModulePhysics.h"

#include "External/Bullet/include/btBulletDynamicsCommon.h"
#include "External/ImGui/imgui.h"

#include <string>

CCollider::CCollider(GameObject* owner) : Component(owner, ComponentType::COLLIDER)
{
	this->mOwner = owner;

	// Default to BOX (y me permito el lujazo)
	collType = ColliderType::BOX;

	SetBoxCollider();
}

CCollider::~CCollider()
{
	//delete shape;
}

void CCollider::Update()
{
	
}

void CCollider::OnInspector()
{
	char* titles[]{ "Box", "Sphere", "Cylinder", "Convex", "Mesh"};
	std::string headerLabel = std::string(titles[*reinterpret_cast<int*>(&collType)]) + " " + "Collider"; // label = "Collider Type" + Collider
	
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	if (ImGui::CollapsingHeader(headerLabel.c_str(), flags))
	{
		ImGui::Indent();

		ImGui::Text("Shape: "); ImGui::SameLine();
		if (ImGui::Combo("##Collider Type", reinterpret_cast<int*>(&collType), titles, IM_ARRAYSIZE(titles))) 
		{
			switch (collType)
			{
			case ColliderType::BOX:
				SetBoxCollider();
				break;
			case ColliderType::SPHERE:
				SetSphereCollider();
				break;
			case ColliderType::CYLINDER:
				SetCylinderCollider();
				break;
			case ColliderType::CONVEX_HULL:
				SetConvexCollider();
				break;
			case ColliderType::MESH_COLLIDER:
				SetMeshCollider();
				break;
			}
		}

		ImGui::Unindent();
	}
}

btCollisionShape* CCollider::GetShape()
{
	return shape;
}

// Setters ----------------------------------------------------------------------
void CCollider::SetBoxCollider()
{
	//collType = ColliderType::BOX;

	//shape = new btBoxShape(btVector3(2.0f, 2.0f, 2.0f));
	//collider = new btCollisionObject();
	//collider->setCollisionShape(shape);

	//btTransform transform;
	//transform.setIdentity();
	//collider->setWorldTransform(transform);

	//External->physics->world->addCollisionObject(collider);
}

void CCollider::SetSphereCollider()
{

}

void CCollider::SetCylinderCollider()
{

}

void CCollider::SetConvexCollider()
{

}

void CCollider::SetMeshCollider()
{

}