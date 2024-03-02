#include "CCollider.h"
#include "GameObject.h"
#include "ModulePhysics.h"

#include "External/Bullet/include/btBulletDynamicsCommon.h"
#include "External/ImGui/imgui.h"

#include <string>

CCollider::CCollider(GameObject* owner) : Component(owner, ComponentType::PHYSICS)
{
	this->mOwner = owner;

	// Default to BOX (y me permito el lujazo)
	collType = ColliderType::BOX;

	SetBoxCollider();

	physType = physicsType::DYNAMIC;
	mass = 1;
}

CCollider::~CCollider()
{
	//delete shape;
}

void CCollider::Update()
{
	if (TimeManager::gameTimer.GetState() == TimerState::RUNNING)
	{
		if (collider != nullptr) {
			float matrix[16];
		
			physBody->GetTransform(matrix);
			mOwner->mTransform->SetTransformFromMatrix(matrix);
			mOwner->mTransform->UpdateTransformsChilds();
		}
	}
}

void CCollider::OnInspector()
{
	char* titles[]{ "Box", "Sphere", "Cylinder", "Convex", "Mesh"};
	std::string headerLabel = std::string(titles[*reinterpret_cast<int*>(&collType)]) + " " + "Collider"; // label = "Collider Type" + Collider
	
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	if (ImGui::CollapsingHeader(headerLabel.c_str(), flags))
	{
		ImGui::Indent();

		ImGui::Spacing();
		ImGui::SeparatorText("COLLIDER");
		ImGui::Spacing();

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


		// -----------------------------------------------------------------------------------------------------


		ImGui::Spacing();
		ImGui::SeparatorText("RIGIDBODY");
		ImGui::Spacing();

		ImGui::PushItemWidth(120.0f);

		const char* items[] = { "Dynamic", "Kinematic", "Static" };
		int currentItem = static_cast<int>(physType);

		ImGui::Text("Physics Type: "); ImGui::SameLine();
		ImGui::Combo("##Physics Type", &currentItem, items, IM_ARRAYSIZE(items));
		physType = static_cast<physicsType>(currentItem);

		if (physType != physicsType::STATIC)
		{
			ImGui::Text("Mass: "); ImGui::SameLine();
			if (ImGui::DragFloat("##Mass", &mass))
				External->physics->SetBodyMass(physBody, mass);
			
		}
		if (physType == physicsType::DYNAMIC)
			ImGui::Checkbox("Use gravity\t", &gravity);

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
	LOG("Set Box Collider");
	collType = ColliderType::BOX;

	CCube cube;
	cube.size.x = 3;
	cube.size.y = 3;
	cube.size.z = 3;
	
	transform = mOwner->mTransform;
	if (transform) {
		float3 pos = transform->GetGlobalPosition();
		cube.SetPos(pos.x / 2, pos.y / 2 / 4, pos.z / 2);
	}
	physBody = External->physics->AddBody(cube, 1);
	mass = 1;

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