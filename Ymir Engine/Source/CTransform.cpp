#include "CTransform.h"

#include "External/ImGui/imgui.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"
#include "External/ImGui/backends/imgui_impl_opengl3.h"

#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"

#include "External/mmgr/mmgr.h"

CTransform::CTransform(GameObject* owner) : Component(owner, ComponentType::TRANSFORM)
{
	translation = float3::zero;
	eulerRot = float3::zero;
	rotation = Quat::FromEulerXYZ(eulerRot[0] * DEGTORAD, eulerRot[1] * DEGTORAD, eulerRot[2] * DEGTORAD);
	scale = float3::one;

	//translationPtr = nullptr;
	//rotationPtr = nullptr;
	//scalePtr = nullptr;

	resetPressed = false;

	mLocalMatrix = float4x4::identity;
	mGlobalMatrix = float4x4::identity;

	dirty_ = true;
}

CTransform::CTransform(GameObject* g, float3 pos, Quat rot, float3 sc, bool start_enabled)
{
	translation = pos;
	rotation = rot;
	eulerRot = rotation.ToEulerXYZ();
	scale = sc;

	mGlobalMatrix = float4x4::FromTRS(pos, rot, sc);
	mLocalMatrix = float4x4::identity;

	resetPressed = false;

	dirty_ = true;
}

CTransform::~CTransform()
{ 

}

void CTransform::Update()
{
	if (dirty_)
	{
		UpdateTransformsChilds();
	}
}

void CTransform::OnInspector()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	if (ImGui::CollapsingHeader("Transform", flags))
	{
		ImGui::Indent();

		if (ImGui::DragFloat3("Position", &translation[0], 0.1f))
		{
			SetPosition(translation);
		}

		if (ImGui::DragFloat3("Rotation", &eulerRot[0], 0.1f))
		{
			SetRotation(eulerRot);
		}

		if (ImGui::DragFloat3("Scale", &scale[0], 0.1f))
		{
			SetScale(scale);
		}

		if (ImGui::Button("Reset Transform")) 
		{
			SetTransform(float3(0, 0, 0), float3(0, 0, 0), float3(1, 1, 1));
		}

		ImGui::Unindent();
	}

}

void CTransform::SetTransform(float3 pos, float3 rot, float3 scale)
{
	SetPosition(pos);
	SetRotation(rot);
	SetScale(scale);
}

void CTransform::SetTransform(CTransform& t)
{
	SetPosition(t.translation);
	SetRotation(t.eulerRot);
	SetScale(t.scale);
}

void CTransform::SetTransformFromMatrix(const float* matrix)
{
	translation = float3(matrix[12], matrix[13], matrix[14]);

	float3 extractedScale;
	extractedScale.x = float3(matrix[0], matrix[1], matrix[2]).Length();
	extractedScale.y = float3(matrix[4], matrix[5], matrix[6]).Length();
	extractedScale.z = float3(matrix[8], matrix[9], matrix[10]).Length();
	scale = extractedScale;

	rotation.x = atan2(matrix[6], matrix[10]) * RADTODEG;
	rotation.y = atan2(-matrix[2], sqrt(matrix[1] * matrix[1] + matrix[0] * matrix[0])) * RADTODEG;
	rotation.z = atan2(matrix[1], matrix[0]) * RADTODEG;

	// Recalcular la matriz local
	CalculateMatrix();
}

void CTransform::CalculateMatrix()
{
	float rx = rotation.x * DEGTORAD;
	float ry = rotation.y * DEGTORAD;
	float rz = rotation.z * DEGTORAD;

	Quat q;
	q = Quat::FromEulerXYZ(rx, ry, rz);
	mGlobalMatrix = float4x4::FromTRS(translation, q, scale).Transposed();
}

void CTransform::SetPosition(float3 vec)
{
	translation = static_cast<float3>(vec);
	dirty_ = true;
}

void CTransform::SetRotation(float3 vec)
{
	rotation = Quat::FromEulerXYZ(vec[0] * DEGTORAD, vec[1] * DEGTORAD, vec[2] * DEGTORAD);
	dirty_ = true;
}

void CTransform::SetOrientation(btQuaternion bulletQuat)
{
	rotation = math::Quat(bulletQuat.x(), bulletQuat.y(), bulletQuat.z(), bulletQuat.w());
	dirty_ = true;
}

void CTransform::SetScale(float3 vec)
{
	scale = float3(vec);
	dirty_ = true;
}

float4x4 CTransform::GetGlobalTransform() const
{
	return mGlobalMatrix;
}

float4x4 CTransform::GetLocalTransform() const
{
	return mLocalMatrix;
}

float3 CTransform::GetGlobalPosition() const
{
	float3 pos, sc;
	Quat rot;
	mGlobalMatrix.Decompose(pos, rot, sc);

	return pos;
}

Quat CTransform::GetLocalRotation() const
{
	return rotation;
}

Quat CTransform::GetGlobalRotation() const
{
	float3 pos, sc;
	Quat rot;
	mGlobalMatrix.Decompose(pos, rot, sc);

	return rot;
}

void CTransform::UpdateTransformsChilds()
{
	UpdateGlobalMatrix();

	if (!mOwner->mChildren.empty())
	{
		for (auto i = 0; i < mOwner->mChildren.size(); i++)
		{
			mOwner->mChildren[i]->mTransform->UpdateTransformsChilds();
		}
	}
	dirty_ = false;
}

void CTransform::UpdateGlobalMatrix()
{
	UpdateLocalMatrix();

	if (mOwner->mParent != nullptr && mOwner->mParent->mTransform != nullptr)
	{
		float4x4 Global_parent = mOwner->mParent->mTransform->mGlobalMatrix;
		mGlobalMatrix = Global_parent * mLocalMatrix;// Your global matrix = your parent�s global matrix * your local Matrix
	}

	else
	{
		mGlobalMatrix = mLocalMatrix;
	}

	UpdateBoundingBoxes();
}

void CTransform::UpdateLocalMatrix()
{
	mLocalMatrix = float4x4::FromTRS(translation, rotation, scale);
	eulerRot = rotation.ToEulerXYZ();
	eulerRot *= RADTODEG;
}

void CTransform::ReparentTransform(float4x4 matrix)
{
	matrix.Decompose(translation, rotation, scale);
	eulerRot = rotation.ToEulerXYZ();
	eulerRot *= RADTODEG;
	dirty_ = true;
}

void CTransform::UpdateBoundingBoxes()
{
	CMesh* meshComponent = (CMesh*)(*mOwner).GetComponent(ComponentType::MESH);

	if (meshComponent != nullptr)
	{
		meshComponent->obb = meshComponent->aabb;
		meshComponent->obb.Transform(mOwner->mTransform->GetGlobalTransform());
		meshComponent->globalAABB.SetNegativeInfinity();
		meshComponent->globalAABB.Enclose(meshComponent->obb);
	}
}

void CTransform::UpdateTransformGuizmo(float4x4 matrix)
{
	mGlobalMatrix = matrix;
	if (mOwner->mParent->mTransform != nullptr)
	{
		float4x4 parentGlobal = mOwner->mParent->mTransform->mGlobalMatrix;
		parentGlobal.Inverse();

		mLocalMatrix = parentGlobal * mGlobalMatrix;
	}
	else
	{
		mLocalMatrix = mGlobalMatrix;
	}

	mLocalMatrix.Decompose(translation, rotation, scale);
	eulerRot = rotation.ToEulerXYZ();
	eulerRot *= RADTODEG;

	dirty_ = true;
}

float3 CTransform::GetForward()
{
	return GetNormalizeAxis(2);
}

float3 CTransform::GetUp()
{
	return GetNormalizeAxis(1);
}

float3 CTransform::GetRight()
{
	return GetNormalizeAxis(0);
}

float3 CTransform::GetLocalForward()
{
	return GetNormalizeLocalAxis(2);
}

float3 CTransform::GetLocalUp()
{
	return GetNormalizeLocalAxis(1);
}

float3 CTransform::GetLocalRight()
{
	return GetNormalizeLocalAxis(0);
}

float3 CTransform::GetNormalizeAxis(int i)
{
	return mGlobalMatrix.RotatePart().Col(i).Normalized();
}

float3 CTransform::GetNormalizeLocalAxis(int i)
{
	return mLocalMatrix.RotatePart().Col(i).Normalized();
}
