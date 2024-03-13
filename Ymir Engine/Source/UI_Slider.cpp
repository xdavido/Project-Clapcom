#include "UI_Slider.h"
#include "GameObject.h"
#include "G_UI.h"

#include "UI_Canvas.h"
#include "ModuleEditor.h"

#include "External/mmgr/mmgr.h"

UI_Slider::UI_Slider(GameObject* g, float x, float y, G_UI* fill, G_UI* handle, float w, float h) : C_UI(UI_TYPE::SLIDER, ComponentType::UI, g, "Slider", x, y, w, h)
{
	isInteractable = true;

	focusedColor = { 0, 1, 0, 1 };
	pressedColor = { 1, 0, 1, 1 };
	selectedColor = { 0, 0, 1, 1 };
	disabledColor = { 1, 1, 1, 1 };

	fillImage = fill;
	fillImage->vReferences.push_back(this);

	handleImage = handle;
	handleImage->vReferences.push_back(this);

	minValue.iValue = 0;
	maxValue.iValue = 1;

	value.iValue = 0;
	useFloat = false;

	direction = SLIDER_DIRECTION::LEFT_TO_RIGHT;
	usingBar = false;
	usingHandle = false;
}

UI_Slider::~UI_Slider()
{
}

update_status UI_Slider::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (fillImage != nullptr && usingBar)
	{
		//SliderBar(dt);
	}

	if (handleImage != nullptr && usingHandle)
	{
		SliderHandle(dt);
	}

	return ret;
}

void UI_Slider::OnInspector()
{
	bool exists = true;
	ImGui::Checkbox(("##UI Slider Checkbox" + std::to_string(mOwner->UID)).c_str(), &active);		ImGui::SameLine();

	if (ImGui::CollapsingHeader(("Slider##UI Collapsing Header" + std::to_string(mOwner->UID)).c_str(), &exists, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (!active) { ImGui::BeginDisabled(); }

		ImGui::Dummy(ImVec2(0, 10));

		if (ImGui::Checkbox("Interactable", &isInteractable))
		{
			if (!isInteractable)
			{
				//static_cast<UI_Image*>(handleImage->GetComponentUI(UI_TYPE::IMAGE))->color = disabledColor;
			}
		} ImGui::SameLine();

		ImGui::Dummy(ImVec2(0, 10));

		ImGui::SeparatorText("Colors");
		ImGui::ColorEdit4("Normal color", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Focused color", (float*)&focusedColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Pressed color", (float*)&pressedColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Selected color", (float*)&selectedColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Disabled color", (float*)&disabledColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

		ImGui::Dummy(ImVec2(0, 10));

		ImGui::SeparatorText("Game Objects references");
		ImGui::Dummy(ImVec2(0, 10));

		//
		ImGui::Text("Fill Image: ");	ImGui::SameLine();
		fillImage = static_cast<G_UI*>(ImGui_GameObjectReference(fillImage));

		// Handle Image reference
		ImGui::Text("Handle Image: ");	ImGui::SameLine();
		handleImage = static_cast<G_UI*>(ImGui_GameObjectReference(handleImage));

		ImGui::Dummy(ImVec2(0, 10));

		ImGui::Separator();

		ImGui::Dummy(ImVec2(0, 10));

		const char* directions[]{ "Left to Right", "Right to Left", "Bottom to Up", "Up to Bottom" };
		if (ImGui::Combo("Direction", (int*)&direction, directions, IM_ARRAYSIZE(directions)));

		ImGui::Dummy(ImVec2(0, 10));

		if (ImGui::Checkbox("Use Floats", &useFloat))
		{
			if (useFloat)
			{
				minValue.fValue = (float)minValue.iValue;
				maxValue.fValue = (float)maxValue.iValue;

				value.fValue = (float)value.iValue;
			}
			else
			{
				minValue.iValue = (int)minValue.fValue;
				maxValue.iValue = (int)maxValue.fValue;

				value.iValue = (int)value.fValue;
			}
		}

		if (useFloat)
		{
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.3);
			ImGui::DragFloat("Min Value", &minValue.fValue);
			ImGui::DragFloat("Max Value", &maxValue.fValue);
			ImGui::PopItemWidth();

			if (minValue.fValue == maxValue.fValue)
			{
				ImGui::TextColored(ImVec4(1, 0, 1, 1), "[!]");
				ImGui::SameLine();
				ImGui::Text("Min value and Max value can't be the same");
			}

			if (minValue.fValue > maxValue.fValue)
			{
				ImGui::TextColored(ImVec4(1, 0, 1, 1), "[!]");
				ImGui::SameLine();
				ImGui::Text("Min value can't be bigger than Max value");
			}

			ImGui::SliderFloat("Value", &value.fValue, minValue.fValue, maxValue.fValue, "%.2f");
		}
		else
		{
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.3);
			ImGui::DragInt("Min Value", &minValue.iValue);
			ImGui::DragInt("Max Value", &maxValue.iValue);
			ImGui::PopItemWidth();

			if (minValue.iValue == maxValue.iValue)
			{
				ImGui::TextColored(ImVec4(1, 0, 1, 1), "[!]");
				ImGui::SameLine();
				ImGui::Text("Min value and Max value can't be the same");
			}

			if (minValue.iValue > maxValue.iValue)
			{
				ImGui::TextColored(ImVec4(1, 0, 1, 1), "[!]");
				ImGui::SameLine();
				ImGui::Text("Min value can't be bigger than Max value");
			}

			ImGui::SliderInt("Value", &value.iValue, minValue.iValue, maxValue.iValue);
		}

		if (!active) { ImGui::EndDisabled(); }
	}
	ImGui::SameLine();

	if (!exists) { mOwner->RemoveComponent(this); }
}

void UI_Slider::OnReferenceDestroyed(void* ptr)
{
	if (fillImage == ptr)
	{
		fillImage = nullptr;
	}
	else if (handleImage == ptr)
	{
		handleImage = nullptr;
	}

}

void UI_Slider::OnNormal()
{
	if (isInteractable)
	{
		//image->color = color;
	}
	else
	{
		//image->color = disabledColor;
	}
}

void UI_Slider::OnFocused()
{
	//image->color = focusedColor;
}

void UI_Slider::OnPressed()
{
	//image->color = pressedColor;

	if (fillImage != nullptr && fillImage->active && usingBar)
	{
		usingBar = true;
	}

	if (handleImage != nullptr && handleImage->active)
	{
		usingHandle = true;
	}
}

void UI_Slider::OnSelected()
{
	//image->color = selectedColor;

	if (fillImage != nullptr)
	{

	}

	if (handleImage != nullptr)
	{
		// if moving joystick --> drag true, else drag false
		//if ()
		//{
			//handleImage->GetComponentUI(UI_TYPE::IMAGE)->isDragging = true;
		//}
	}
}

void UI_Slider::OnRelease()
{
	if (fillImage != nullptr)
	{
		usingBar = false;
	}

	if (handleImage != nullptr)
	{
		usingHandle = false;
	}
}

void UI_Slider::SliderBar(float dt)
{
	int movementX = External->input->GetMouseXMotion() * dt * 30;
	int movementY = -External->input->GetMouseYMotion() * dt * 30;

	C_UI* comp = fillImage->GetComponentUI(UI_TYPE::IMAGE);

	if (comp->width + movementX >= dragLimits.x && comp->width + movementX <= dragLimits.x + dragLimits.z &&
		comp->height + movementY >= dragLimits.y && comp->height + movementY <= dragLimits.y + dragLimits.w)
	{

		if (direction == SLIDER_DIRECTION::LEFT_TO_RIGHT || direction == SLIDER_DIRECTION::RIGHT_TO_LEFT)
		{
			comp->width += movementX;
			(useFloat ? value.fValue = dragLimits.z * 100 / comp->width : value.iValue = dragLimits.z * 100 / comp->width);
		}
		else
		{
			comp->height += movementY;
			(useFloat ? value.fValue = dragLimits.z * 100 / comp->width : value.iValue = dragLimits.z * 100 / comp->width);
		}

		float3 globalPos;
		Quat rot;
		float3 scale;

		mOwner->mTransform->SetPosition(float3(mOwner->mTransform->translation.x + movementX, mOwner->mTransform->translation.y + movementY, 0));
		mOwner->mTransform->mGlobalMatrix.Decompose(globalPos, rot, scale);

		scaleBounds = scale;

		float3 position = mOwner->mTransform->translation;

		boundsEditor->vertices[0].position = float3(position.x + movementX, position.y + (comp->height * scaleBounds.y) + movementY, 0);
		boundsEditor->vertices[1].position = float3(position.x + (comp->width * scaleBounds.x) + movementX, position.y + (comp->height * scaleBounds.y) + movementY, 0);
		boundsEditor->vertices[2].position = float3(position.x + movementX, position.y + movementY, 0);
		boundsEditor->vertices[3].position = float3(position.x + (comp->width * scaleBounds.x) + movementX, position.y + movementY, 0);

		boundsGame->vertices[0].position = float3(posX, posY + (comp->height * scaleBounds.y), 0);
		boundsGame->vertices[1].position = float3(posX + (comp->width * scaleBounds.x), posY + (comp->height * scaleBounds.y), 0);
		boundsGame->vertices[2].position = float3(posX, posY, 0);
		boundsGame->vertices[3].position = float3(posX + (comp->width * scaleBounds.x), posY, 0);


		boundsEditor->RegenerateVBO();
		boundsGame->RegenerateVBO();
	}
}

void UI_Slider::SliderHandle(float dt)
{
	int movementX = External->input->GetMouseXMotion() * dt * 30;
	int movementY = -External->input->GetMouseYMotion() * dt * 30;

	if (posX + movementX >= dragLimits.x && posX + movementX <= dragLimits.x + dragLimits.z &&
		posY + movementY >= dragLimits.y && posY + movementY <= dragLimits.y + dragLimits.w)
	{
		if (direction == SLIDER_DIRECTION::LEFT_TO_RIGHT || direction == SLIDER_DIRECTION::RIGHT_TO_LEFT)
		{
			posX += movementX;
			(useFloat ? value.fValue = dragLimits.z * 100 / posX : value.iValue = dragLimits.z * 100 / posX);
		}
		else
		{
			posY += movementY;
			(useFloat ? value.fValue = dragLimits.z * 100 / posX : value.iValue = dragLimits.z * 100 / posX);
		}

		float3 globalPos;
		Quat rot;
		float3 scale;

		mOwner->mTransform->SetPosition(float3(mOwner->mTransform->translation.x + movementX, mOwner->mTransform->translation.y + movementY, 0));
		mOwner->mTransform->mGlobalMatrix.Decompose(globalPos, rot, scale);

		scaleBounds = scale;

		float3 position = mOwner->mTransform->translation;

		boundsEditor->vertices[0].position = float3(position.x + movementX, position.y + (height * scaleBounds.y) + movementY, 0);
		boundsEditor->vertices[1].position = float3(position.x + (width * scaleBounds.x) + movementX, position.y + (height * scaleBounds.y) + movementY, 0);
		boundsEditor->vertices[2].position = float3(position.x + movementX, position.y + movementY, 0);
		boundsEditor->vertices[3].position = float3(position.x + (width * scaleBounds.x) + movementX, position.y + movementY, 0);

		boundsGame->vertices[0].position = float3(posX, posY + (height * scaleBounds.y), 0);
		boundsGame->vertices[1].position = float3(posX + (width * scaleBounds.x), posY + (height * scaleBounds.y), 0);
		boundsGame->vertices[2].position = float3(posX, posY, 0);
		boundsGame->vertices[3].position = float3(posX + (width * scaleBounds.x), posY, 0);


		boundsEditor->RegenerateVBO();
		boundsGame->RegenerateVBO();
	}
}
