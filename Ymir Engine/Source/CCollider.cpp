#include "CCollider.h"
#include "GameObject.h"

#include "External/Bullet/include/btBulletDynamicsCommon.h"
#include "External/ImGui/imgui.h"

#include <string>

CCollider::CCollider(GameObject* owner) : Component(owner, ComponentType::COLLIDER)
{
	this->mOwner = owner;

	// Default to BOX (y me permito el lujazo)
	collType = ColliderType::BOX;
}

CCollider::~CCollider()
{
	delete shape;
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
				break;
			case ColliderType::SPHERE:
				break;
			case ColliderType::CYLINDER:
				break;
			case ColliderType::CONVEX_HULL:
				break;
			case ColliderType::MESH_COLLIDER:
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