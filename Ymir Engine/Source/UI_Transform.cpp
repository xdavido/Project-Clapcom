#include "UI_Canvas.h"

#include "UI_Transform.h"

#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"

#include "External/mmgr/mmgr.h"

UI_Transform::UI_Transform(C_UI* owner) : Component(owner->mOwner, ComponentType::UI_TRAMSFORM)
{
	componentReference = owner;

	mMatrixUI = float4x4::identity;
	dirty_ = true;

}

UI_Transform::~UI_Transform()
{
	/*RELEASE(boundsEditorReference);
	RELEASE(boundsGameReference);
	RELEASE(posX);
	RELEASE(posY);
	RELEASE(width);
	RELEASE(height);*/
}


void UI_Transform::OnInspector()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	if (ImGui::CollapsingHeader("UI Transform", flags))
	{
		ImGui::Indent();

		if (ImGui::DragFloat("Position X", &componentReference->posX, 0.1f))
		{
			dirty_ = true;
		}

		if (ImGui::DragFloat("Position Y", &componentReference->posY, 0.1f))
		{
			dirty_ = true;
		}

		if (ImGui::DragFloat("Width", &componentReference->width, 0.1f))
		{
			dirty_ = true;
		}

		if (ImGui::DragFloat("Height", &componentReference->height, 0.1f))
		{
			dirty_ = true;
		}

		if (ImGui::DragFloat3("Scale", &componentReference->scaleBounds[0], 0.1f))
		{
			dirty_ = false;
		}

		ImGui::Unindent();
	}

}



update_status UI_Transform::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return ret;
}



