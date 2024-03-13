#include "UI_Button.h"
#include "GameObject.h"
#include "G_UI.h"

#include "UI_Canvas.h"
#include "ModuleEditor.h"

#include "External/mmgr/mmgr.h"

UI_Button::UI_Button(GameObject* g, float x, float y, float w, float h) : C_UI(UI_TYPE::BUTTON, ComponentType::UI, g, "Button", x, y, w, h)
{
	isInteractable = true;

	focusedColor = { 0, 1, 0, 1 };
	pressedColor = { 1, 0, 1, 1 };
	selectedColor = { 0, 0, 1, 1 };
	disabledColor = { 1, 1, 1, 1 };

	image = nullptr;
	displayText = nullptr;
}

UI_Button::~UI_Button()
{
	// Image is already another GameObject
	image = nullptr;
}

void UI_Button::OnInspector()
{
	bool exists = true;
	ImGui::Checkbox(("##UI Button Checkbox" + std::to_string(mOwner->UID)).c_str(), &active);		ImGui::SameLine();

	if (ImGui::CollapsingHeader(("Button##UI Collapsing Header" + std::to_string(mOwner->UID)).c_str(), &exists, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (!active) { ImGui::BeginDisabled(); }

		ImGui::Dummy(ImVec2(0, 10));

		if (ImGui::Checkbox("Interactable", &isInteractable))
		{
			if (!isInteractable && image != nullptr)
			{
				image->color = disabledColor;
			}
		} ImGui::SameLine();
		ImGui::Checkbox("Draggeable", &isDraggeable);

		// Image reference
		ImGui::Text("Image: ");	ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ENGINE_COLOR);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ENGINE_COLOR);
		if (ImGui::Button(image == nullptr ? "<null>" : (image->mOwner)->name.c_str()))
		{
			/*if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
				{
					handleImage = static_cast<G_UI*>(External->editor->draggedGO);
				}
				ImGui::EndDragDropTarget();
			}*/

			if (image->mOwner != nullptr)
			{
				External->scene->SetSelected(image->mOwner);
			}
		}
		ImGui::PopStyleColor(2);

		ImGui::Dummy(ImVec2(0, 10));

		// Text reference
		ImGui::Text("Text: ");	ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ENGINE_COLOR);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ENGINE_COLOR);
		if (ImGui::Button(displayText == nullptr ? "<null>" : (displayText->mOwner)->name.c_str()))
		{
			/*if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
				{
					handleImage = static_cast<G_UI*>(External->editor->draggedGO);
				}
				ImGui::EndDragDropTarget();
			}*/

			if (displayText->mOwner != nullptr)
			{
				External->scene->SetSelected(displayText->mOwner);
			}
		}
		ImGui::PopStyleColor(2);
		ImGui::Dummy(ImVec2(0, 10));

		// Colors
		ImGui::SeparatorText("Colors");
		ImGui::ColorEdit4("Normal color", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Focused color", (float*)&focusedColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Pressed color", (float*)&pressedColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Selected color", (float*)&selectedColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Disabled color", (float*)&disabledColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

		{
			ImGui::Dummy(ImVec2(0, 10));
			ImGui::Text("Funtion: "); ImGui::SameLine();

			ImGui::Text("<null>");
		}

		if (!active) { ImGui::EndDisabled(); }
	}
	ImGui::SameLine();

	if (!exists) { mOwner->RemoveComponent(this); }
}

void UI_Button::OnReferenceDestroyed(void* ptr)
{
	if (image->mOwner == ptr)
	{
		image = nullptr;
	}
	else if (displayText->mOwner == ptr)
	{
		displayText = nullptr;
	}
}

void UI_Button::OnNormal()
{
	if (isInteractable)
	{
		image->color = color;
	}
	else
	{
		image->color = disabledColor;
	}
}

void UI_Button::OnFocused()
{
	image->color = focusedColor;
}

void UI_Button::OnPressed()
{
	image->color = pressedColor;
}

void UI_Button::OnSelected()
{
	image->color = selectedColor;
}

void UI_Button::OnRelease()
{

}
