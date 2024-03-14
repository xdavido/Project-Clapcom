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
	maxValue.iValue = 10;

	value.iValue = 0;
	useFloat = false;

	direction = SLIDER_DIRECTION::LEFT_TO_RIGHT;
	usingBar = false;
	usingHandle = false;

	// Dragging
	dragLimits.x = x;
	dragLimits.y = y;
	dragLimits.z = w;
	dragLimits.w = h;
}

UI_Slider::~UI_Slider()
{
}

update_status UI_Slider::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	dragLimits.x = mOwner->mTransform->GetGlobalPosition().x;
	dragLimits.y = mOwner->mTransform->GetGlobalPosition().y;

	if (External->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
	{
		usingBar = false;
		usingHandle = false;
	}

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

	if (fillImage != nullptr && fillImage->active)
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
	UI_Image& img = *(UI_Image*)handleImage->GetComponentUI(UI_TYPE::IMAGE);

	//if (img.state == UI_STATE::PRESSED)
	{
		int movementX = External->input->GetMouseXMotion() * dt * 30;
		int movementY = -External->input->GetMouseYMotion() * dt * 30;		

		// Get the Mouse Position using ImGui.
		ImVec2 mousePosition = ImGui::GetMousePos();

		// Get the position of the ImGui window.
		ImVec2 sceneWindowPos = External->editor->gameViewPos;

		// Get the size of the ImGui window.
		ImVec2 sceneWindowSize = External->editor->gameViewSize;

		ImVec2 normalizedPoint;
		normalizedPoint.x = mousePosition.x - sceneWindowPos.x;
		normalizedPoint.y = mousePosition.y - sceneWindowPos.y;

		// Calculate new position of the slider handle
		float newX = normalizedPoint.x + movementX;
		float newY = normalizedPoint.y + movementY;

		// Calculate new position of the slider handle
		//float newX = External->input->GetMouseX() + movementX;
		//float newY = External->input->GetMouseY() + movementY;

		LOG("%f - %f", newX, newY);

		// Check if the new position is within dragLimits
		if (newX >= dragLimits.x && newX <= dragLimits.x + dragLimits.z &&
			newY >= dragLimits.y && newY <= dragLimits.y + dragLimits.w)
		{
			if (direction == SLIDER_DIRECTION::LEFT_TO_RIGHT || direction == SLIDER_DIRECTION::RIGHT_TO_LEFT)
			{
				LOG("pos %f", img.posX);
				img.posX = newX;
				movementY = 0;

				ValueCalculationsFromHandles(img.posX, dragLimits.x + dragLimits.z);

				float3 position = img.mOwner->mTransform->translation;
				
				img.boundsEditor->vertices[0].position = float3(position.x + movementX, position.y + (img.height * img.scaleBounds.y) + movementY, 0);
				img.boundsEditor->vertices[1].position = float3(position.x + (img.width * img.scaleBounds.x) + movementX, position.y + (img.height * img.scaleBounds.y) + movementY, 0);
				img.boundsEditor->vertices[2].position = float3(position.x + movementX, position.y + movementY, 0);
				img.boundsEditor->vertices[3].position = float3(position.x + (img.width * img.scaleBounds.x) + movementX, position.y + movementY, 0);
				
				img.boundsGame->vertices[0].position = float3(newX, img.posY + (img.height * img.scaleBounds.y), 0);
				img.boundsGame->vertices[1].position = float3(newX + (img.width * img.scaleBounds.x), img.posY + (img.height * img.scaleBounds.y), 0);
				img.boundsGame->vertices[2].position = float3(newX, img.posY, 0);
				img.boundsGame->vertices[3].position = float3(newX + (img.width * img.scaleBounds.x), img.posY, 0);
			}
			else
			{
				img.posY = newY;
				movementX = 0;

				ValueCalculationsFromHandles(img.posY, dragLimits.y + dragLimits.w);
			}

			// Top left - Top right - Bot left - Bot right
			/*boundsEditor->vertices[0].position = float3(position.x + movementX, position.y + (height * scaleBounds.y) + movementY, 0);
			boundsEditor->vertices[1].position = float3(position.x + (width * scaleBounds.x) + movementX, position.y + (height * scaleBounds.y) + movementY, 0);
			boundsEditor->vertices[2].position = float3(position.x + movementX, position.y + movementY, 0);
			boundsEditor->vertices[3].position = float3(position.x + (width * scaleBounds.x) + movementX, position.y + movementY, 0);

			img.boundsGame->vertices[0].position = float3(newX, img.posY + (height * scaleBounds.y), 0);
			img.boundsGame->vertices[1].position = float3(newX + (width * scaleBounds.x), img.posY + (height * scaleBounds.y), 0);
			img.boundsGame->vertices[2].position = float3(newX, img.posY, 0);
			img.boundsGame->vertices[3].position = float3(newX + (width * scaleBounds.x), img.posY, 0);*/

			img.boundsEditor->RegenerateVBO();
			img.boundsGame->RegenerateVBO();
		}
	}
}

void UI_Slider::ValueCalculationsFromHandles(float val, float max)
{
	if (useFloat)
	{
		float aux = val * maxValue.fValue / max;

		if (aux < minValue.fValue)
		{
			value.fValue = minValue.fValue;
		}
		else if (aux > maxValue.fValue)
		{
			value.fValue = maxValue.fValue;
		}
		else
		{
			value.fValue = aux;
		}
	}
	else
	{
		int aux = val * maxValue.iValue / (dragLimits.x + dragLimits.z);

		if (aux < minValue.iValue)
		{
			value.iValue = minValue.iValue;
		}
		else if (aux > maxValue.iValue)
		{
			value.iValue = maxValue.iValue;
		}
		else
		{
			value.iValue = aux;
		}
	}
}
