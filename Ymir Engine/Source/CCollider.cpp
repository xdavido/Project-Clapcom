#include "CCollider.h"
#include "GameObject.h"
#include "ModulePhysics.h"

#include "External/Bullet/include/btBulletDynamicsCommon.h"
#include "External/ImGui/imgui.h"
#include "TimeManager.h"

#include <string>
#include <vector>
#include "External/ImGuizmo/include/ImGuizmo.h"

CCollider::CCollider(GameObject* owner) : Component(owner, ComponentType::PHYSICS)
{
	this->mOwner = owner;

	// Default to BOX (y me permito el lujazo)
	collType = ColliderType::BOX;
	mass = 1;
	size = float3{ 1,1,1 };
	btSize = float3_to_btVector3(size);

	physType = physicsType::DYNAMIC;

	SetBoxCollider();

	// Get info from obb

	CMesh* componentMesh = (CMesh*)mOwner->GetComponent(ComponentType::MESH);

	if (componentMesh != nullptr) {

		size = componentMesh->rMeshReference->obb.Size();

	}
	else {

		size = float3(10, 10, 10);

	}

	shape->setLocalScaling(btSize);

	convexShape = nullptr;
	
}

CCollider::~CCollider()
{
	External->physics->world->removeRigidBody(physBody->body);

	delete physBody;
	delete shape;
	delete convexShape;
}

void CCollider::Update()
{
	if (TimeManager::gameTimer.GetState() == TimerState::RUNNING)
	{
		if (physBody != nullptr) {

			float matrix[16];
			physBody->GetTransform(matrix);

			CMesh* componentMesh = (CMesh*)mOwner->GetComponent(ComponentType::MESH);
			CTransform* componentTransform = (CTransform*)mOwner->GetComponent(ComponentType::TRANSFORM);

			float offsetX = componentMesh->rMeshReference->obb.CenterPoint().x - componentTransform->GetGlobalPosition().x;
			float offsetY = componentMesh->rMeshReference->obb.CenterPoint().y - componentTransform->GetGlobalPosition().y;
			float offsetZ = componentMesh->rMeshReference->obb.CenterPoint().z - componentTransform->GetGlobalPosition().z;

			float4x4 newMat;

			newMat.SetCol(0, float4(matrix[0], matrix[1], matrix[2], matrix[3]));
			newMat.SetCol(1, float4(matrix[4], matrix[5], matrix[6], matrix[7]));
			newMat.SetCol(2, float4(matrix[8], matrix[9], matrix[10], matrix[11]));
			newMat.SetCol(3, float4(matrix[12] - offsetX, matrix[13] - offsetY, matrix[14] - offsetZ, matrix[15]));

			mOwner->mTransform->SetPosition(newMat.TranslatePart());
			//mOwner->mTransform->SetRotation(newMat.RotatePart().ToEulerXYZ());
			//mOwner->mTransform->SetScale(newMat.GetScale());

			mOwner->mTransform->UpdateTransformsChilds();

		}
		
	}

	if (TimeManager::gameTimer.GetState() == TimerState::STOPPED && active) {

		CMesh* componentMesh = (CMesh*)mOwner->GetComponent(ComponentType::MESH);
		CTransform* componentTransform = (CTransform*)mOwner->GetComponent(ComponentType::TRANSFORM);

		if (componentMesh != nullptr && physBody != nullptr) {

			float3 pos = componentMesh->rMeshReference->obb.CenterPoint();
			physBody->SetPosition(pos);

			physBody->SetRotation(componentTransform->GetLocalRotation());

			if (ImGuizmo::IsUsing()) {

				size = componentMesh->rMeshReference->obb.Size();

			}

		}

	}	

	btSize = float3_to_btVector3(size);
	shape->setLocalScaling(btSize);

}

void CCollider::OnInspector()
{
	char* titles[]{ "Box", "Sphere", "Cylinder", "Convex (UNSTABLE)", "Mesh"};
	std::string headerLabel = std::string(titles[*reinterpret_cast<int*>(&collType)]) + " " + "Collider"; // label = "Collider Type" + Collider
	
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	ImGui::Checkbox(("##" + mOwner->name + std::to_string(ctype)).c_str(), &active);
	ImGui::SameLine();

	bool exists = true;

	if (ImGui::CollapsingHeader(headerLabel.c_str(), &exists, flags))
	{
		ImGui::Indent();

		ImGui::Spacing();

		if (!active) { ImGui::BeginDisabled(); }

		ImGui::SeparatorText("COLLIDER");
		ImGui::Spacing();

		ImGui::Text("Shape: "); ImGui::SameLine();
		if (ImGui::Combo("##Collider Type", reinterpret_cast<int*>(&collType), titles, IM_ARRAYSIZE(titles))) 
		{
			switch (collType)
			{
			case ColliderType::BOX:
				RemovePhysbody();
				SetBoxCollider();
				break;
			case ColliderType::SPHERE:
				RemovePhysbody();
				SetSphereCollider();
				break;
			case ColliderType::CYLINDER:
				RemovePhysbody();
				SetCylinderCollider();
				break;
			case ColliderType::CONVEX_HULL:
				RemovePhysbody();
				SetConvexCollider();
				break;
			case ColliderType::MESH_COLLIDER:
				RemovePhysbody();
				SetMeshCollider();
				break;
			}
		}

		if (shape != nullptr) {

			ImGui::Text("Scale: "); ImGui::SameLine();

			ImGui::DragFloat3("##Scale", size.ptr(), 0.1f, 0.1f);

		}

		if (ImGui::Button("Reset Collider Size")) {

			CMesh* componentMesh = (CMesh*)mOwner->GetComponent(ComponentType::MESH);

			if (componentMesh != nullptr) {

				size = componentMesh->rMeshReference->obb.Size();

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
		if (ImGui::Combo("##Physics Type", &currentItem, items, IM_ARRAYSIZE(items)))
		{
			physType = static_cast<physicsType>(currentItem);
			SetDefaultValues(physType);
			External->physics->RecalculateInertia(physBody, mass, gravity);
		}

		switch (physType) 
		{
		case physicsType::DYNAMIC:
			ImGui::Text("Mass: "); ImGui::SameLine();
			if (ImGui::DragFloat("##Mass", &mass, 1.0f, 0.0f, 1000.0f))
				External->physics->RecalculateInertia(physBody, mass, gravity);
			if (ImGui::Checkbox("Use gravity\t", &gravity))
				External->physics->RecalculateInertia(physBody, mass, gravity);

            break;

        case physicsType::KINEMATIC:
			ImGui::Text("Mass: "); ImGui::SameLine();
			if (ImGui::DragFloat("##Mass", &mass, 1.0f, 0.0f, 1000.0f))
				External->physics->RecalculateInertia(physBody, mass, false);
            break;

        case physicsType::STATIC:			
            break;

        default:
            break;
		}

		if (!active) { ImGui::EndDisabled(); }

		ImGui::Unindent();

	}

	if (!exists) { mOwner->RemoveComponent(this); }

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
	cube.size.x = size.x;
	cube.size.y = size.y;
	cube.size.z = size.z;
	
	transform = mOwner->mTransform;

	if (transform) {

		float3 pos = transform->GetGlobalPosition();
		cube.SetPos(pos.x /*/ 2*/, pos.y /*/ 2 / 4*/, pos.z /*/ 2*/);
	}

	physBody = External->physics->AddBody(cube, physicsType::DYNAMIC, mass, true, shape);

}

void CCollider::SetSphereCollider()
{

}

void CCollider::SetCylinderCollider()
{

}

void CCollider::SetConvexCollider()
{
	LOG("Set Convex Collider");
	collType = ColliderType::CONVEX_HULL;

	CTransform* componentTransform = (CTransform*)mOwner->GetComponent(ComponentType::TRANSFORM);
	CMesh* componentMesh = (CMesh*)mOwner->GetComponent(ComponentType::MESH);

	physBody = External->physics->AddBody(componentMesh, physicsType::DYNAMIC, mass, true, convexShape);

	physBody->SetPosition(componentTransform->GetGlobalPosition());
	//float3 size = auxMesh->rMeshReference->globalAABB.CenterPoint();
	////convexShape->setLocalScaling(btVector3(size.x, size.y, size.z));
	//convexShape->setImplicitShapeDimensions(btVector3(size.x, size.y, size.z));
}

void CCollider::SetMeshCollider()
{

}

void CCollider::SetDefaultValues(physicsType type)
{
	switch (physType)
	{
	case physicsType::DYNAMIC:
		mass = 1;
		gravity = true;
		break;
	case physicsType::KINEMATIC:
		mass = 1;
		gravity = false;
		break;
	case physicsType::STATIC:
		mass = 0;
		gravity = false;
		break;
	default:
		mass = 0;
		gravity = false;
		break;
	}
}

void CCollider::RemovePhysbody()
{
	if (physBody != nullptr) {
		External->physics->RemoveBody(physBody);
		physBody = nullptr;
	}
}