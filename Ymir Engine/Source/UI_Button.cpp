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

	tabNav_ = true;
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
		ImGui::Text(("Image: " + (image == nullptr ? "<null>" : image->mat->diffuse_path)).c_str());	//ImGui::SameLine();

		ImGui::Text(("State: " + std::to_string((int)state)).c_str());

		// Text reference
		ImGui::Text("Text: ");	ImGui::SameLine();
		displayText = static_cast<G_UI*>(ImGui_GameObjectReference(displayText));

		ImGui::Dummy(ImVec2(0, 10));

		// Colors
		//ImGui::SeparatorText("Colors");
		//ImGui::ColorEdit4("Normal color", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		//ImGui::ColorEdit4("Focused color", (float*)&focusedColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		//ImGui::ColorEdit4("Pressed color", (float*)&pressedColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		//ImGui::ColorEdit4("Selected color", (float*)&selectedColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		//ImGui::ColorEdit4("Disabled color", (float*)&disabledColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

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

void UI_Button::SetReference()
{
	std::map<std::string, int>::iterator it = vTempReferences.begin();

	while (it != vTempReferences.end())
	{
		if (it->first == "Text")
		{
			displayText = (G_UI*)mOwner->FindChild(it->second);
			if (displayText != nullptr)
			{
				displayText->vReferences.push_back(this);
			}
		}
		else if (it->first == "Image")
		{
			image = (UI_Image*)static_cast<G_UI*>(mOwner)->GetComponentUI(UI_TYPE::IMAGE);
			if (image != nullptr)
			{
				image->mOwner->vReferences.push_back(this);
			}

			image->SetImg(mPaths[UI_STATE::NORMAL], UI_STATE::NORMAL);
			image->SetImg(mPaths[UI_STATE::FOCUSED], UI_STATE::FOCUSED);
			image->SetImg(mPaths[UI_STATE::PRESSED], UI_STATE::PRESSED);
			image->SetImg(mPaths[UI_STATE::SELECTED], UI_STATE::SELECTED);
			image->SetImg(mPaths[UI_STATE::RELEASE], UI_STATE::RELEASE);
			image->SetImg(mPaths[UI_STATE::DISABLED], UI_STATE::DISABLED);
		}

		++it;
	}
}

void UI_Button::OnReferenceDestroyed(void* ptr)
{
	if (image->mOwner == ptr)
	{
		image = nullptr;
	}
	else if (displayText == ptr)
	{
		displayText = nullptr;
	}
}

void UI_Button::OnNormal()
{
	if (isInteractable)
	{
		image->color = color;
		image->selectedTexture = image->mapTextures.find(state)->second;
	}
	else
	{
		image->color = disabledColor;
	}
}

void UI_Button::OnFocused()
{
	image->color = focusedColor;
	image->selectedTexture = image->mapTextures.find(state)->second;

	if (mOwner != nullptr)
	{
		CScript* aux = dynamic_cast<CScript*>(mOwner->GetComponent(ComponentType::SCRIPT));

		if (aux != nullptr) {
			aux->OnHoverButton();
		}
	}
}

void UI_Button::OnPressed()
{
	image->color = pressedColor;
	image->selectedTexture = image->mapTextures.find(state)->second;
}

void UI_Button::OnSelected()
{
	image->color = selectedColor;
	image->selectedTexture = image->mapTextures.find(state)->second;
}

void UI_Button::OnRelease()
{
	image->selectedTexture = image->mapTextures.find(state)->second;

	if (mOwner != nullptr) 
	{
		CScript* aux = dynamic_cast<CScript*>(mOwner->GetComponent(ComponentType::SCRIPT));

		if (aux != nullptr) {
			aux->OnClickButton();
		}
	}
}

void UI_Button::OnDisabled()
{
	image->selectedTexture = image->mapTextures.find(state)->second;
}