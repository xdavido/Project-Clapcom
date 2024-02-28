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
	delete collider;
}

void CCollider::Update()
{

}

void CCollider::OnInspector()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	ImGui::Checkbox(("##" + mOwner->name + std::to_string(ctype)).c_str(), &active);
	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Collider", flags))
	{
		ImGui::Indent();

		//TODO: Inspector

		ImGui::Unindent();
	}
}