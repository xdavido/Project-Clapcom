#include "UI_Slider.h"
#include "GameObject.h"
#include "G_UI.h"

#include "UI_Canvas.h"
#include "ModuleEditor.h"

#include "External/mmgr/mmgr.h"

#define ENGINE_COLOR ImVec4(1, 0.5, 0.5, 1)

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
}

UI_Slider::~UI_Slider()
{
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

	if (fillImage != nullptr)
	{

	}

	if (handleImage != nullptr)
	{
		handleImage->GetComponentUI(UI_TYPE::IMAGE)->isDragging = true;
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
		{
			handleImage->GetComponentUI(UI_TYPE::IMAGE)->isDragging = true;
		}
	}
}

void UI_Slider::OnRelease()
{
	if (fillImage != nullptr)
	{

	}

	if (handleImage != nullptr)
	{
		handleImage->GetComponentUI(UI_TYPE::IMAGE)->isDragging = false;
	}
}
