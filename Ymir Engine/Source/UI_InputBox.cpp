#include "UI_InputBox.h"
#include "GameObject.h"

#include "ModuleInput.h"

#include "External/ImGui/imgui.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"
#include "External/ImGui/backends/imgui_impl_opengl3.h"
#include "External/ImGui/misc/cpp/imgui_stdlib.h"
//
//#include "External/ImGui/imgui_custom.h"

#include "External/mmgr/mmgr.h"

UI_InputBox::UI_InputBox(GameObject* g, float x, float y, float w, float h) : C_UI(UI_TYPE::INPUTBOX, ComponentType::UI, g, "Input Box", x, y, w, h)
{
	displayText = nullptr;
	fontSize = 21;
	image = nullptr;

	focusedColor = { 0, 1, 0, 1 };
	pressedColor = { 1, 0, 1, 1 };
	selectedColor = { 1.0f, 0.081f, 1.0f, 1.0f };
	disabledColor = { 1, 1, 1, 1 };

	text = "uwu";
	maxChars = 10;

	isWriting = false;
	tabNav_ = true;
}

UI_InputBox::~UI_InputBox()
{
	displayText = nullptr;
	image = nullptr;
}

update_status UI_InputBox::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (state == UI_STATE::NORMAL)
	{
		isWriting = false;
		External->input->SetInputActive(text, false);
	}
	
	if (displayText != nullptr && text != displayText->text) { displayText->text = text; }

	return ret;
}

void UI_InputBox::OnInspector()
{
	bool exists = true;
	ImGui::Checkbox(("##UI Input Box Checkbox" + std::to_string(mOwner->UID)).c_str(), &active);		ImGui::SameLine();

	if (ImGui::CollapsingHeader(("Input Box##UI Collapsing Header" + std::to_string(mOwner->UID)).c_str(), &exists, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (!active) { ImGui::BeginDisabled(); }

		ImGui::Checkbox("Draggeable", &isDraggeable);

		//ImGuiCustom::ToggleButton("Writing##", &isWriting);

		ImGui::Text("Text");
		ImGui::InputTextMultiline("##input", &text, ImVec2(0, 0), ImGuiInputTextFlags_AllowTabInput);

		ImGui::Dummy(ImVec2(0, 10));

		ImGui::Text("Characters limit");
		if (ImGui::DragInt("##Characters limit", (maxChars < 0) ? &(maxChars = 0) : &maxChars))
		{
			External->input->SetMaxChars(maxChars);
		}

		ImGui::SeparatorText("Colors");
		ImGui::ColorEdit4("Normal color", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Selected color", (float*)&selectedColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4("Disabled color", (float*)&disabledColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

		if (!active) { ImGui::EndDisabled(); }
	}
	ImGui::SameLine();

	if (!exists) { mOwner->RemoveComponent(this); }
}

void UI_InputBox::OnNormal()
{
	image->color = color;
}

void UI_InputBox::OnFocused()
{
	image->color = focusedColor;
}

void UI_InputBox::OnPressed()
{
	image->color = pressedColor;
}

void UI_InputBox::OnSelected()
{
	if (!isWriting)
	{
		isWriting = true;
		External->input->SetInputActive(text);

		image->color = selectedColor;
	}
	else if (!External->input->GetInputActive())
	{
		isWriting = false;
		state = UI_STATE::NORMAL;
	}
}

void UI_InputBox::OnRelease()
{
}
