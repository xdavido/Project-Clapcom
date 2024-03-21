#include "CCollider.h"
#include "GameObject.h"
#include "ModulePhysics.h"

#include "External/Bullet/include/btBulletDynamicsCommon.h"
#include "External/ImGui/imgui.h"
#include "TimeManager.h"

#include <string>
#include <vector>
#include "External/ImGuizmo/include/ImGuizmo.h"

#include "External/mmgr/mmgr.h"

CCollider::CCollider(GameObject* owner, ColliderType collider, PhysicsType physics) : Component(owner, ComponentType::PHYSICS)
{
	this->mOwner = owner;

	collType = collider;
	physType = physics;

	isSensor = false;

	mass = 1;
	size = float3{ 1,1,1 };
	btSize = float3_to_btVector3(size);
	radius = 1;

	lockX = false;
	lockY = false;
	lockZ = false;

	switch (collType)
	{
	case BOX:
		SetBoxCollider();
		break;
	case SPHERE:
		SetSphereCollider();
		break;
	case CAPSULE:
		SetCapsuleCollider();
		break;
	case MESH_COLLIDER:
		SetMeshCollider();
		break;
	default:
		break;
	}

	// Get info from obb

	CMesh* componentMesh = (CMesh*)mOwner->GetComponent(ComponentType::MESH);

	if (componentMesh != nullptr) {

		size = componentMesh->rMeshReference->obb.Size();
		radius = size.Length() / 2;
		height = size.y;
	}
	else {

		size = float3(10, 10, 10);
		radius = 5;
		height = 10;
	}

	//shape->setLocalScaling(btSize);
	
}

CCollider::~CCollider()
{
	RemovePhysbody();

	delete physBody;
	delete shape;
}

void CCollider::Update()
{
	if (External->physics->beginPlay)
		if (physBody != nullptr) 	External->physics->RecalculateInertia(physBody, mass, useGravity);

	if (TimeManager::gameTimer.GetState() == TimerState::RUNNING)
	{
		if (physBody != nullptr) {

			float matrix[16];
			physBody->GetTransform(matrix);

			CMesh* componentMesh = (CMesh*)mOwner->GetComponent(ComponentType::MESH);
			CTransform* componentTransform = (CTransform*)mOwner->GetComponent(ComponentType::TRANSFORM);

			CTransform* parentTransform = (CTransform*)mOwner->mParent->GetComponent(ComponentType::TRANSFORM);

			float offsetX = 0;
			float offsetY = 0;
			float offsetZ = 0;

			if (componentMesh != nullptr) {
				offsetX = componentMesh->rMeshReference->obb.CenterPoint().x - componentTransform->GetGlobalPosition().x;
				offsetY = componentMesh->rMeshReference->obb.CenterPoint().y - componentTransform->GetGlobalPosition().y;
				offsetZ = componentMesh->rMeshReference->obb.CenterPoint().z - componentTransform->GetGlobalPosition().z;
			}

			float4x4 newMat;

			newMat.SetCol(0, float4(matrix[0], matrix[1], matrix[2], matrix[3]));
			newMat.SetCol(1, float4(matrix[4], matrix[5], matrix[6], matrix[7]));
			newMat.SetCol(2, float4(matrix[8], matrix[9], matrix[10], matrix[11]));

			if (mOwner->mParent == nullptr)
			{
				if (collType != ColliderType::MESH_COLLIDER)
					newMat.SetCol(3, float4(matrix[12] - offsetX, matrix[13] - offsetY, matrix[14] - offsetZ, matrix[15]));
				else
					newMat.SetCol(3, float4(matrix[12], matrix[13], matrix[14], matrix[15]));
			}
			else
			{
				if (collType != ColliderType::MESH_COLLIDER)
					newMat.SetCol(3, float4(matrix[12] - offsetX - parentTransform->GetGlobalPosition().x, 
											matrix[13] - offsetY - parentTransform->GetGlobalPosition().y, 
											matrix[14] - offsetZ - parentTransform->GetGlobalPosition().z,  
											matrix[15]));
				else
					newMat.SetCol(3, float4(matrix[12] - parentTransform->GetGlobalPosition().x, 
											matrix[13] - parentTransform->GetGlobalPosition().y, 
											matrix[14] - parentTransform->GetGlobalPosition().z, 
											matrix[15]));
			}

			mOwner->mTransform->SetPosition(newMat.TranslatePart());
			mOwner->mTransform->SetOrientation(physBody->body->getOrientation());
			//mOwner->mTransform->SetScale(newMat.GetScale());

			mOwner->mTransform->UpdateTransformsChilds();

		}

	}

	if (TimeManager::gameTimer.GetState() == TimerState::STOPPED && active) {

		CMesh* componentMesh = (CMesh*)mOwner->GetComponent(ComponentType::MESH);
		CTransform* componentTransform = (CTransform*)mOwner->GetComponent(ComponentType::TRANSFORM);

		if (componentMesh != nullptr && physBody != nullptr) {

			float3 pos;

			if (collType != ColliderType::MESH_COLLIDER)
				pos = componentMesh->rMeshReference->obb.CenterPoint();
			else
				pos = componentTransform->GetGlobalPosition();

			physBody->SetPosition(pos);
			physBody->SetRotation(componentTransform->GetLocalRotation());

			//physBody->SetScale(componentTransform->scale);
			/*physBody->SetScale*/
			if (ImGuizmo::IsUsing()) {

				size = componentMesh->rMeshReference->obb.Size();
				radius = size.Length() / 2;
				height = size.y;
			}

		}
		else {

			physBody->SetPosition(componentTransform->GetGlobalTransform().TranslatePart());
			physBody->SetRotation(componentTransform->GetGlobalRotation());

			if (ImGuizmo::IsUsing()) {

				size = componentTransform->GetGlobalTransform().GetScale();

			}

		}

	}

	if (size.x == 0) size.x = 0.1;
	if (size.y == 0) size.y = 0.1;
	if (size.z == 0) size.z = 0.1;

	btSize = float3_to_btVector3(size);
	shape->setLocalScaling(btSize);
}

void CCollider::OnInspector()
{
	char* titles[]	{ "Box", "Sphere", "Capsule", "Mesh" };

	std::string headerLabel = std::string(titles[*reinterpret_cast<int*>(&collType)]) + " " + "Collider"; // label = "Collider Type" + Collider
	
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	bool exists = true;

	ImGui::Checkbox(("##" + std::to_string(UID)).c_str(), &active);
	ImGui::SameLine();

	if (ImGui::CollapsingHeader((headerLabel + "##" + std::to_string(UID)).c_str(), &exists, flags))
	{
		ImGui::Indent();

		ImGui::Spacing();

		if (!active) { ImGui::BeginDisabled(); }

		ImGui::SeparatorText("COLLIDER");
		ImGui::Spacing();


		bool auxIsSensor = isSensor;
		if (ImGui::Checkbox("Is Sensor", &auxIsSensor))
		{
			SetAsSensor(auxIsSensor);
		}

		if (ImGui::Checkbox("Draw Shape", &physBody->drawShape))
		{
			!physBody->drawShape;
		}

		ImGui::Text("Shape: "); ImGui::SameLine();
		
		int hasNotMesh = 0;
		if (mOwner->GetComponent(ComponentType::MESH) == nullptr) hasNotMesh = 1; // if mesh = false -> value = 1

		if (ImGui::Combo("##Collider Type", reinterpret_cast<int*>(&collType), titles, IM_ARRAYSIZE(titles) - hasNotMesh)) 
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
			case ColliderType::CAPSULE:
				RemovePhysbody();
				SetCapsuleCollider();
				break;
			case ColliderType::MESH_COLLIDER:
				RemovePhysbody();
				SetMeshCollider();
				break;
			default:
				break;
			}
		}

		if (shape != nullptr) {

			switch (collType)
			{
			case ColliderType::BOX: 

				ImGui::Text("Scale: "); ImGui::SameLine();

				ImGui::DragFloat3("##Scale", size.ptr(), 0.1f, 0.1f);

				break;

			case ColliderType::SPHERE:

				ImGui::Text("Radius: "); ImGui::SameLine();

				if (ImGui::DragFloat("##Radius", &radius, 0.1f, 0.1f))
				{
					size.x = radius;
					size.y = radius;
					size.z = radius;
				}

				break;
			case ColliderType::CAPSULE:
				ImGui::Text("Radius: "); ImGui::SameLine();
				if (ImGui::DragFloat("##Radius", &radius, 0.1f, 0.1f))
				{
					if (radius == 0) radius = 0.1f;

					size.x = radius;
					size.z = radius;
				}

				ImGui::Text("Height: "); ImGui::SameLine();
				ImGui::DragFloat("##height", &size.y, 0.1f, 0.1f);

				break;
			}

			if (ImGui::Button("Reset Collider Size")) {

				CMesh* componentMesh = (CMesh*)mOwner->GetComponent(ComponentType::MESH);

				if (componentMesh != nullptr) {

					size = componentMesh->rMeshReference->obb.Size();
					radius = size.Length();
					height = size.y;
				}

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
			physType = static_cast<PhysicsType>(currentItem);
			SetDefaultValues(physType);
			External->physics->RecalculateInertia(physBody, mass, useGravity);
		}

		switch (physType) 
		{
		case PhysicsType::DYNAMIC:
			ImGui::Text("Mass: "); ImGui::SameLine();
			if (ImGui::DragFloat("##Mass", &mass, 1.0f, 0.0f, 1000.0f))
				External->physics->RecalculateInertia(physBody, mass, useGravity);
			if (ImGui::Checkbox("Use gravity\t", &useGravity))
				External->physics->RecalculateInertia(physBody, mass, useGravity);

			ImGui::Spacing();
			ImGui::Text("Lock rotation");

			if (ImGui::Checkbox("Lock x\t", &lockX))
				UpdateLockRotation();

			ImGui::SameLine();
			if (ImGui::Checkbox("Lock y\t", &lockY))
				UpdateLockRotation();
				
			ImGui::SameLine();
			if (ImGui::Checkbox("Lock z\t", &lockZ))
				UpdateLockRotation();

            break;

        case PhysicsType::KINEMATIC:
			ImGui::Text("Mass: "); ImGui::SameLine();
			if (ImGui::DragFloat("##Mass", &mass, 1.0f, 0.0f, 1000.0f))
				External->physics->RecalculateInertia(physBody, mass, false);
            break;

        case PhysicsType::STATIC:			
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
	//cube.size.x = size.x;
	//cube.size.y = size.y;
	//cube.size.z = size.z;
	
	transform = mOwner->mTransform;

	//if (transform) {

	//	float3 pos = transform->GetGlobalPosition();
	//	cube.SetPos(pos.x, pos.y, pos.z);
	//}

	physBody = External->physics->AddBody(cube, PhysicsType::DYNAMIC, mass, true, shape);

}

void CCollider::SetSphereCollider()
{
	LOG("Set Sphere Collider");
	collType = ColliderType::SPHERE;

	CSphere sphere;

	transform = mOwner->mTransform;

	physBody = External->physics->AddBody(sphere, PhysicsType::DYNAMIC, mass, true, shape);
}

void CCollider::SetCapsuleCollider()
{
	LOG("Set Capsule Collider");
	collType = ColliderType::CAPSULE;
	
	CCapsule capsule;

	transform = mOwner->mTransform;

	physBody = External->physics->AddBody(capsule, PhysicsType::DYNAMIC, mass, true, shape);
}

void CCollider::SetMeshCollider()
{
	CMesh* auxMesh = (CMesh*)mOwner->GetComponent(ComponentType::MESH);
	if (auxMesh == nullptr)
	{
		LOG("Mesh collider needs a ComponentMesh!");
		SetBoxCollider();
		return;
	}

	LOG("Set Mesh Collider");
	collType = ColliderType::MESH_COLLIDER;


	physBody = External->physics->AddBody(auxMesh, PhysicsType::DYNAMIC, mass, true, shape);

	//float3 size = auxMesh->rMeshReference->globalAABB.CenterPoint();
	////convexShape->setLocalScaling(btVector3(size.x, size.y, size.z));
	//convexShape->setImplicitShapeDimensions(btVector3(size.x, size.y, size.z));
}

void CCollider::SetDefaultValues(PhysicsType type)
{
	switch (physType)
	{
	case PhysicsType::DYNAMIC:
		mass = 1;
		useGravity = true;
		break;
	case PhysicsType::KINEMATIC:
		mass = 1;
		useGravity = false;
		break;
	case PhysicsType::STATIC:
		mass = 0;
		useGravity = false;
		break;
	default:
		mass = 0;
		useGravity = false;
		break;
	}
}

void CCollider::UpdateLockRotation()
{
	btVector3 rot;

	if (lockX) rot.setX(0);
	else rot.setX(1);

	if (lockY) rot.setY(0);
	else rot.setY(1);

	if (lockZ) rot.setZ(0);
	else rot.setZ(1);

	physBody->body->setAngularFactor(rot);

}

void CCollider::RemovePhysbody()
{
	if (physBody != nullptr) {
		External->physics->RemoveBody(physBody);
		physBody = nullptr;
	}
}

void CCollider::SetAsSensor(bool is_sensor)
{
	isSensor = is_sensor;

	if (isSensor == true)
	{
		physBody->body->setCollisionFlags(physBody->body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

		physType = PhysicsType::STATIC;
		SetDefaultValues(physType);
		External->physics->RecalculateInertia(physBody, mass, useGravity);
	}
	else
		physBody->body->setCollisionFlags(physBody->body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
}