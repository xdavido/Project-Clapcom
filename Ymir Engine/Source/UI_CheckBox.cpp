#include "UI_Checkbox.h"
#include "GameObject.h"
#include "G_UI.h"
#include "UI_Image.h"

//#include "External/ImGui/imgui_custom.h"

#include "External/mmgr/mmgr.h"

UI_Checkbox::UI_Checkbox(GameObject* g, int x, int y, int w, int h) : C_UI(UI_TYPE::CHECKBOX, ComponentType::UI, g, "Checkbox", x, y, w, h)
{
	isInteractable = true;
	isChecked = false;

	focusedColor = { 0, 1, 0, 1 };
	pressedColor = { 1, 0, 1, 1 };
	selectedColor = { 1.0f, 0.081f, 1.0f, 1.0f };
	disabledColor = { 1, 1, 1, 1 };

	bgImg = nullptr;
	cmImg = nullptr;

	displayText = nullptr;

	tabNav_ = true;
}

UI_Checkbox::~UI_Checkbox()
{
	// Image is already another GameObject
	bgImg = nullptr;
	cmImg = nullptr;
}

void UI_Checkbox::OnInspector()
{
	bool exists = true;
	ImGui::Checkbox(("##UI Checkbox Checkbox" + std::to_string(mOwner->UID)).c_str(), &active);		ImGui::SameLine();

	if (ImGui::CollapsingHeader(("Checkbox##UI Header" + std::to_string(mOwner->UID)).c_str(), &exists, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (!active) { ImGui::BeginDisabled(); }

		ImGui::Dummy(ImVec2(0, 10));

		if (ImGui::Checkbox("Interactable", &isInteractable))
		{
			if (!isInteractable)
			{
				static_cast<UI_Image*>(bgImg->GetComponentUI(UI_TYPE::IMAGE))->color = disabledColor;
				static_cast<UI_Image*>(cmImg->GetComponentUI(UI_TYPE::IMAGE))->color = disabledColor;
			}
		}	ImGui::SameLine();

		ImGui::Checkbox("Draggeable", &isDraggeable);

		if (ImGui::Checkbox("Checked##", &isChecked))
		{
			static_cast<UI_Image*>(cmImg->GetComponentUI(UI_TYPE::IMAGE))->active = isChecked;
		}

		ImGui::Dummy(ImVec2(0, 10));

		(displayText != nullptr) ? ImGui::Text("Text: %s", static_cast<UI_Text*>(displayText->GetComponentUI(UI_TYPE::TEXT))->text.c_str()) : ImGui::Text("Text: <null>");
		ImGui::Dummy(ImVec2(0, 10));

		ImGui::SeparatorText("Colors");
		ImGui::ColorEdit4("Normal color", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Focused color", (float*)&focusedColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Pressed color", (float*)&pressedColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Selected color", (float*)&selectedColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Disabled color", (float*)&disabledColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

		{
			ImGui::Dummy(ImVec2(0, 10));
			ImGui::Text("Funtion: "); ImGui::SameLine();
			//if (defaultFunction1)
			//{
			//	ImGui::Text("void CheckVSync()");
			//}
			//else if (defaultFunction2)
			//{
			//	// TODO: set draggeable
			//	ImGui::Text("void FadeUI(float dtd)");
			//}
			//else
			//{
			//	ImGui::Text("<null>");
			//}
		}

		if (!active) { ImGui::EndDisabled(); }
	}
	ImGui::SameLine();

	if (!exists) { mOwner->RemoveComponent(this); }
}

void UI_Checkbox::OnNormal()
{
	if (isInteractable)
	{
		static_cast<UI_Image*>(bgImg->GetComponentUI(UI_TYPE::IMAGE))->color = color;
		static_cast<UI_Image*>(cmImg->GetComponentUI(UI_TYPE::IMAGE))->color = color;
	}
	else
	{
		static_cast<UI_Image*>(bgImg->GetComponentUI(UI_TYPE::IMAGE))->color = disabledColor;
		static_cast<UI_Image*>(cmImg->GetComponentUI(UI_TYPE::IMAGE))->color = disabledColor;
	}
}

void UI_Checkbox::OnFocused()
{
	static_cast<UI_Image*>(bgImg->GetComponentUI(UI_TYPE::IMAGE))->color = focusedColor;
}

void UI_Checkbox::OnPressed()
{
	static_cast<UI_Image*>(bgImg->GetComponentUI(UI_TYPE::IMAGE))->color = pressedColor;
}

void UI_Checkbox::OnSelected()
{
	static_cast<UI_Image*>(bgImg->GetComponentUI(UI_TYPE::IMAGE))->color = selectedColor;
}

void UI_Checkbox::OnRelease()
{
	if (isInteractable)
	{
		isChecked = !isChecked;
		static_cast<UI_Image*>(cmImg->GetComponentUI(UI_TYPE::IMAGE))->active = isChecked;
	}
}

void UI_Checkbox::CheckDraggeable()
{
	LOG("drag");

	if (static_cast<G_UI*>(mOwner->mParent)->GetComponentUI(UI_TYPE::IMAGE) != nullptr)
	{
		static_cast<G_UI*>(mOwner->mParent)->GetComponentUI(UI_TYPE::IMAGE)->isDraggeable = isChecked;
	}
}
