#include "CCollider.h"
#include "GameObject.h"
#include "ModulePhysics.h"

#include "External/Bullet/include/btBulletDynamicsCommon.h"
#include "External/ImGui/imgui.h"
#include "TimeManager.h"

#include <string>

CCollider::CCollider(GameObject* owner) : Component(owner, ComponentType::PHYSICS)
{
	this->mOwner = owner;

	// Default to BOX (y me permito el lujazo)
	collType = ColliderType::BOX;
	mass = 1;
	size = { 1,1,1 };

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

	shape->setLocalScaling(btVector3(size.x, size.y, size.z));
	
}

CCollider::~CCollider()
{
	External->physics->world->removeRigidBody(physBody->body);

	delete physBody;
	delete shape;
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

	if (TimeManager::gameTimer.GetState() == TimerState::STOPPED && active) {

		CMesh* componentMesh = (CMesh*)mOwner->GetComponent(ComponentType::MESH);

		if (componentMesh != nullptr) {

			float3 pos = componentMesh->rMeshReference->obb.CenterPoint();
			physBody->SetPos(pos.x, pos.y, pos.z);

		}

	}	
	
	/*CTransform* componentTransform = (CTransform*)mOwner->GetComponent(ComponentType::TRANSFORM);
	physBody->SetTransform(componentTransform->GetGlobalTransform().ptr());*/
}

void CCollider::OnInspector()
{
	char* titles[]{ "Box", "Sphere", "Cylinder", "Convex", "Mesh"};
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

		if (shape != nullptr) {

			ImGui::Text("Scale: "); ImGui::SameLine();

			if (ImGui::DragFloat3("##Scale", size.ptr(), 0.1f, 0.1f)) {

				shape->setLocalScaling(btVector3(size.x, size.y, size.z));

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
