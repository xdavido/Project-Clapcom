#include "UI_Image.h"
#include "GameObject.h"

#include "Application.h"
#include "ModuleEditor.h"

//#include "External/ImGui/imgui_custom.h"

UI_Image::UI_Image(GameObject* g, int x, int y, int w, int h) : C_UI(UI_TYPE::IMAGE, ComponentType::UI, g, "Image", x, y, w, h)
{
	mat = new CMaterial(g);
}

UI_Image::~UI_Image()
{
	RELEASE(mat);
}

void UI_Image::OnInspector()
{
	bool exists = true;
	ImGui::Checkbox(("##UI Image Checkbox" + std::to_string(mOwner->UID)).c_str(), &active);		ImGui::SameLine();

	if (ImGui::CollapsingHeader(("Image##UI Collapsing Header" + std::to_string(mOwner->UID)).c_str(), &exists, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (!active) { ImGui::BeginDisabled(); }

		ImGui::Checkbox("Draggeable", &isDraggable);
		//if (mat->tex != nullptr)
		//{
		//	// Change Texture
		//	if (ImGui::BeginCombo("##Texture", mat->tex->name.c_str()))
		//	{
		//		for (int i = 0; i < App->resource->vTexturesResources.size(); i++)
		//		{
		//			const bool is_selected = (App->resource->vTexturesResources[i] == mat->tex);
		//			if (ImGui::Selectable(App->resource->vTexturesResources[i]->name.c_str(), is_selected))
		//			{
		//				gameObject->ChangeComponentResource(mat->tex, App->resource->vTexturesResources[i], *mat);
		//			}

		//			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
		//			if (is_selected)
		//			{
		//				ImGui::SetItemDefaultFocus();
		//			}
		//		}
		//		ImGui::EndCombo();
		//	}
		//	ImGui::SameLine();
		//	//ImGuiCustom::HelpMarker("If the current texture has only one instance you will not be able to change it back unless a that texture is imported to scene");
		//}

		//if (ImGuiCustom::ToggleButton("Checkered##", &mat->checkered))
		//{

		//}
		//ImGui::SetItemTooltip("Use checkered texture");

		//if (mat->tex != nullptr)
		//{
		//	if (mat->tex->assetsFile != "")
		//	{
		//		ImGui::Text("Texture Width: %d", mat->tex->tex_width);
		//		ImGui::Text("Texture Height: %d", mat->tex->tex_height);
		//		ImGui::TextWrapped("Texture Path: %s", mat->tex->assetsFile);
		//		ImGui::Dummy(ImVec2(0, 10));
		//	}
		//	else
		//	{
		//		ImGui::Text("Material: NoLambert");
		//	}

		//	ImGui::Image((void*)(intptr_t)mat->tex->tex_id, ImVec2(100, 100));
		//}

		ImGui::ColorEdit4("Color", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

		if (!active) { ImGui::EndDisabled(); }
	}
	ImGui::SameLine();

	// TODO: to test
	if (!exists) 
	{
		//mOwner->ChangeComponentResource(mat->tex, nullptr, *this);
	}
}

void UI_Image::Draw(bool game)
{
//	if (mat != nullptr && mat->tex != nullptr)
//	{
		UIBounds* boundsDrawn = nullptr;

		if (game)
		{
			boundsDrawn = boundsGame;

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.0, External->editor->gameViewSize.x, 0.0, External->editor->gameViewSize.y, 1.0, -1.0);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}

		else
		{
			boundsDrawn = boundsEditor;

			glPushMatrix();
			glMultMatrixf(mOwner->mTransform->mGlobalMatrix.Transposed().ptr());
		}


		//(!mat->checkered) ? glBindTexture(GL_TEXTURE_2D, (mat->tex->tex_id))
		//	: glBindTexture(GL_TEXTURE_2D, App->renderer3D->texture_checker);

		// TODO:  equivalent to this glBindTexture(GL_TEXTURE_2D, itr->second->textureID);

		for (auto& textures : mat->rTextures) {

			textures->BindTexture(true);

		}

		mat->shader.UseShader(true);
		mat->shader.SetShaderUniforms(&mOwner->mTransform->mGlobalMatrix, false);


		// Render
		glBindVertexArray(boundsDrawn->VAO);

		glDrawElements(GL_TRIANGLES, boundsDrawn->indices.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);



		mat->shader.UseShader(false);

		for (auto& textures : mat->rTextures) {

			textures->BindTexture(false);

		}



		if (!game)
		{
			glPopMatrix();
		}

		boundsDrawn = nullptr;
	//}
}

update_status UI_Image::Update(float dt)
{
	return update_status();
}

void UI_Image::SetNativeSize()
{
}
