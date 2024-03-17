#include "UI_Canvas.h"

#include "UI_Transform.h"

#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"

#include "External/mmgr/mmgr.h"

UI_Transform::UI_Transform(C_UI* owner) : Component(owner->mOwner, ComponentType::UI_TRAMSFORM)
{
	componentReference = owner;

	auxPosX = componentReference->posX;
	auxPosY = componentReference->posY;

	mMatrixUI = float4x4::identity;

}

UI_Transform::~UI_Transform()
{
	//RELEASE(boundsEditorReference);
	//RELEASE(boundsGameReference);
}


void UI_Transform::OnInspector()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	if (ImGui::CollapsingHeader("UI Transform", flags))
	{
		ImGui::Indent();

		auxPosX = componentReference->posX;
		auxPosY = componentReference->posY;

		if (ImGui::DragFloat("POSITION X", &componentReference->posX, 1.0f))
		{
			UpdateUITransformChilds();
			componentReference->dirty_ = true;
		}

		if (ImGui::DragFloat("POSITION Y", &componentReference->posY, 1.0f))
		{
			UpdateUITransformChilds();
			componentReference->dirty_ = true;
		}

		if (ImGui::DragFloat("WIDTH", &componentReference->width, 1.0f))
		{
			UpdateUITransformChilds();
			componentReference->dirty_ = true;
		}

		if (ImGui::DragFloat("HEIGHT", &componentReference->height, 1.0f))
		{
			UpdateUITransformChilds();
			componentReference->dirty_ = true;
		}

		if (ImGui::DragFloat3("SCALE", &componentReference->scaleBounds[0], 0.1f))
		{
			UpdateUITransformChilds();
			componentReference->dirty_ = true;
		}

		ImGui::Unindent();
	}

}

void UI_Transform::UpdateUITransformChilds()
{
	std::vector<C_UI*> listUI;
	External->renderer3D->GetUIGOs(mOwner, listUI);

	for (auto i = 1; i < listUI.size(); i++)
	{
		listUI[i]->posX = listUI[i]->posX + componentReference->posX - auxPosX;
		listUI[i]->posY = listUI[i]->posY + componentReference->posY - auxPosY;
		listUI[i]->scaleBounds = componentReference->scaleBounds;
		listUI[i]->dirty_ = true;
	}

}



update_status UI_Transform::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return ret;
}



