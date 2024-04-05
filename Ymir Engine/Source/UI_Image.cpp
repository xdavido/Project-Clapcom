#include "UI_Image.h"
#include "GameObject.h"
#include "UI_Transform.h"

#include "Application.h"
#include "ModuleEditor.h"
#include "ImporterTexture.h"

//#include "External/ImGui/imgui_custom.h"

#include "External/mmgr/mmgr.h"

UI_Image::UI_Image(GameObject* g, float x, float y, float w, float h, std::string imgPath, std::string shaderPath) : C_UI(UI_TYPE::IMAGE, ComponentType::UI, g, "Image", x, y, w, h)
{
	mat = new CMaterial(g);
	mat->shaderPath = shaderPath;
	mat->shader.LoadShader(mat->shaderPath);

	SetImg(imgPath, UI_STATE::NORMAL);

	selectedTexture = mapTextures.find(state)->second;

	ssRows = 1;
	ssColumns = 1;

	ssCoordsX = 0;
	ssCoordsY = 0;

	SetSpriteSize();

	tabNav_ = false;
}

UI_Image::~UI_Image()
{
	RELEASE(mat);
	RELEASE(selectedTexture);
	//RELEASE(mapTextures);
}

void UI_Image::OnInspector()
{
	// Vectors of shader paths and names
	std::vector<const char*> listShaderPaths;
	std::vector<const char*> listShaderNames;

	// Manage loaded shaders
	for (auto& it = Shader::loadedShaders.begin(); it != Shader::loadedShaders.end(); ++it) {

		listShaderPaths.push_back(it->first.c_str());

		std::string shaderFileName = std::filesystem::path(it->first).stem().string();
		listShaderNames.push_back(strdup(shaderFileName.c_str())); // strdup to allocate new memory

	}

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	ImGui::Checkbox(("##" + mOwner->name + std::to_string(ctype)).c_str(), &active);
	ImGui::SameLine();

	bool exists = true;

	if (ImGui::CollapsingHeader("Image", &exists, flags))
	{
		ImGui::Indent();

		ImGui::Spacing();

		if (!active) { ImGui::BeginDisabled(); }

		// ------------------------------------ IMAGE ------------------------------------

		ImGui::SeparatorText("Image");

		ImGui::Spacing();
		ImGui::Text("Image Width: %d", width);
		ImGui::Text("Image Height: %d", height);

		ImGui::Spacing();

		// Change Texture
		if (ImGui::BeginCombo("##Texture", selectedTexture->GetAssetsFilePath().c_str()))
		{
			for (auto& textures : mat->rTextures)
			{
				const bool is_selected = (textures == selectedTexture);
				if (ImGui::Selectable((textures->GetAssetsFilePath() + "##" + std::to_string(textures->GetUID())).c_str(), is_selected))
				{
					selectedTexture = textures;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("Set Native Size", ImVec2(110, 30)))
		{
			SetNativeSize();
		}

		// Current frame
		ImGui::SeparatorText("Current frame");
		ImGui::SetNextItemWidth(70.0f);
		if (ImGui::DragInt("X", &ssCoordsX, 0.05f, 1, 0, "%d", ImGuiSliderFlags_AlwaysClamp))
		{
			SetSpriteSize();
		} ImGui::SameLine();

		ImGui::SetNextItemWidth(70.0f);
		if (ImGui::DragInt("Y", &ssCoordsY, 0.05f, 1, 0, "%d", ImGuiSliderFlags_AlwaysClamp))
		{
			SetSpriteSize();
		}

		// Rows and columns
		ImGui::SetNextItemWidth(70.0f);
		if (ImGui::DragInt("Rows", &ssRows, 0.05f, 1, 0, "%d", ImGuiSliderFlags_AlwaysClamp))
		{
			SetSpriteSize();
		} ImGui::SameLine();

		ImGui::SetNextItemWidth(70.0f);
		if (ImGui::DragInt("Columns", &ssColumns, 0.05f, 1, 0, "%d", ImGuiSliderFlags_AlwaysClamp))
		{
			SetSpriteSize();
		}

		ImGui::Spacing();

		// ------------------------------------ SHADER ------------------------------------

		ImGui::SeparatorText("SHADER");

		ImGui::Spacing();

		ImGui::Text("Shader: ");
		ImGui::SameLine();

		if (mOwner->selected) {

			// Find the index of the current shader path in listShaderPaths
			auto it = std::find(listShaderPaths.begin(), listShaderPaths.end(), mat->shaderPath);

			if (it != listShaderPaths.end()) {

				mat->selectedShader = static_cast<int>(std::distance(listShaderPaths.begin(), it));

			}

		}

		// Choose between the list of shaders
		if (ImGui::Combo("##ChooseShader", &mat->selectedShader, listShaderNames.data(), listShaderNames.size())) {

			mat->shaderDirtyFlag = true;

		}

		if (mat->shaderDirtyFlag) {

			// When shader changes, update the shader path and recompile
			mat->shaderPath = listShaderPaths[mat->selectedShader];
			mat->shader.LoadShader(mat->shaderPath);

			// Reset the dirty flag after handling the change
			mat->shaderDirtyFlag = false;

		}

		ImGui::Spacing();

		// Shader Uniforms Management

		if (mOwner->selected) {

			if (mat->shader.uniforms.size() == 0) {

				ImGui::Text("No editable uniforms.");

			}
			else {

				ImGui::Text("Uniforms:");

			}

			ImGui::Spacing();

			ImGui::Indent();

			// In case the shader has editable uniforms:
			for (auto kt = mat->shader.uniforms.begin(); kt != mat->shader.uniforms.end(); ++kt) {

				std::string label = "##" + kt->name;

				ImGui::Text("%s", kt->name.c_str());
				ImGui::SameLine();

				// Change display according to uniform type
				switch (kt->type)
				{
				case UniformType::boolean:

					ImGui::Checkbox(label.c_str(), (bool*)kt->value.get());

					mat->shader.SetUniformValue(kt->name, (bool*)kt->value.get());

					break;

				case UniformType::i1:

					ImGui::DragInt(label.c_str(), (int*)kt->value.get(), 0.1f);

					mat->shader.SetUniformValue(kt->name, (int*)kt->value.get());

					break;

				case UniformType::i2:

					ImGui::DragInt2(label.c_str(), (int*)kt->value.get(), 0.1f);

					mat->shader.SetUniformValue(kt->name, (int*)kt->value.get());

					break;

				case UniformType::i3:

					ImGui::DragInt3(label.c_str(), (int*)kt->value.get(), 0.1f);

					mat->shader.SetUniformValue(kt->name, (int*)kt->value.get());

					break;

				case UniformType::i4:

					ImGui::DragInt4(label.c_str(), (int*)kt->value.get(), 0.1f);

					mat->shader.SetUniformValue(kt->name, (int*)kt->value.get());

					break;

				case UniformType::f1:

					ImGui::DragFloat(label.c_str(), (float*)kt->value.get(), 0.1f);

					mat->shader.SetUniformValue(kt->name, (float*)kt->value.get());

					break;

				case UniformType::f2:

					ImGui::DragFloat2(label.c_str(), (float*)kt->value.get(), 0.1f);

					mat->shader.SetUniformValue(kt->name, (float*)kt->value.get());

					break;

				case UniformType::f3:

					ImGui::DragFloat3(label.c_str(), (float*)kt->value.get(), 0.1f);

					mat->shader.SetUniformValue(kt->name, (float*)kt->value.get());

					break;

				case UniformType::f4:

					ImGui::DragFloat4(label.c_str(), (float*)kt->value.get(), 0.1f);

					mat->shader.SetUniformValue(kt->name, (float*)kt->value.get());

					break;

				}

			}

			ImGui::Unindent();

		}

		ImGui::Spacing();

		// ------------------------------------ TEXTURES ------------------------------------

		ImGui::SeparatorText("TEXTURES");

		ImGui::Spacing();

		// Display texture maps of the gameobject material

		ImVec2 textureMapSize(20, 20);

		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(mat->ID)), textureMapSize);
		mat->DdsDragDropTarget();
		ImGui::SameLine();
		ImGui::Text("Diffuse");
		ImGui::SameLine();
		ImGui::Text("(%s)", mat->path.c_str());

		ImGui::Spacing();

		ImGui::ColorButton("Specular", ImVec4(0, 0, 0, 0), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, textureMapSize);
		mat->DdsDragDropTarget();
		ImGui::SameLine();
		ImGui::Text("Specular");

		ImGui::Spacing();

		ImGui::ColorButton("Normal", ImVec4(0, 0, 0, 0), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, textureMapSize);
		mat->DdsDragDropTarget();
		ImGui::SameLine();
		ImGui::Text("Normal");

		ImGui::Spacing();

		ImGui::ColorButton("Height", ImVec4(0, 0, 0, 0), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, textureMapSize);
		mat->DdsDragDropTarget();
		ImGui::SameLine();
		ImGui::Text("Height");

		ImGui::Spacing();

		ImGui::ColorButton("Ambient", ImVec4(0, 0, 0, 0), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, textureMapSize);
		mat->DdsDragDropTarget();
		ImGui::SameLine();
		ImGui::Text("Ambient");

		ImGui::Spacing();

		ImGui::ColorButton("Emissive", ImVec4(0, 0, 0, 0), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, textureMapSize);
		mat->DdsDragDropTarget();
		ImGui::SameLine();
		ImGui::Text("Emissive");

		ImGui::Spacing();

		// Utility buttons

		if (ImGui::Button("Apply Checker Texture")) {

			External->renderer3D->ApplyCheckerTexture();

		}

		ImGui::Spacing();

		if (ImGui::Button("Clear Actual Texture")) {

			External->renderer3D->ClearActualTexture();

		}

		if (!active) { ImGui::EndDisabled(); }

		ImGui::Spacing();

		ImGui::Unindent();
	}

	if (!exists) { mOwner->RemoveComponent(this); }
}

void UI_Image::Draw(bool game)
{
	UI_Bounds* boundsDrawn = nullptr;

	selectedTexture->BindTexture(true);

	mat->shader.UseShader(true);
	mat->shader.SetShaderUniforms(&transformUI->mMatrixUI, mOwner->selected);


	if (game)
	{
		boundsDrawn = boundsGame;
	}

	else
	{
		boundsDrawn = boundsEditor;

		/*glPushMatrix();
		glMultMatrixf(mOwner->mTransform->mGlobalMatrix.Transposed().ptr());*/
	}

	// Render
	glBindVertexArray(boundsDrawn->VAO);

	glDrawElements(GL_TRIANGLES, boundsDrawn->indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	if (!game)
	{
		//glPopMatrix();
	}

	boundsDrawn = nullptr;

	mat->shader.UseShader(false);
	selectedTexture->BindTexture(false);
}

update_status UI_Image::Update(float dt)
{
	return update_status();
}

void UI_Image::SetImg(std::string imgPath, UI_STATE state)
{
	ResourceTexture* rTexTemp = new ResourceTexture();
	ImporterTexture::Import(imgPath, rTexTemp);
	rTexTemp->type = TextureType::DIFFUSE;
	rTexTemp->UID = Random::Generate();

	auto itr = mapTextures.find(state);
	if (itr != mapTextures.end())
	{
		mat->rTextures.erase(std::find(mat->rTextures.begin(), mat->rTextures.end(), itr->second));
		mat->rTextures.shrink_to_fit();

		mapTextures.erase(state);
	}

	mat->path = imgPath;
	mat->rTextures.push_back(rTexTemp);

	mapTextures.insert({ state, rTexTemp });
}

void UI_Image::SetNativeSize()
{
	width = selectedTexture->GetWidth();
	height = selectedTexture->GetHeight();

	dirty_ = true;
}

void UI_Image::SetSpriteSize()
{
	//if (selectedTexture != nullptr)
	//{
	//	float2 ssSize = float2(selectedTexture->GetWidth() / ssRows, selectedTexture->GetHeight() / ssColumns);

	//	boundsGame->vertices[0].textureCoordinates = float2(ssCoordsX * ssSize.x, ssCoordsY * ssSize.y + ssSize.y);	// 0, 1
	//	boundsGame->vertices[1].textureCoordinates = float2(ssCoordsX * ssSize.x + ssSize.x, ssCoordsY * ssSize.y + ssSize.y);	// 1, 1
	//	boundsGame->vertices[2].textureCoordinates = float2(ssCoordsX * ssSize.x, ssCoordsY * ssSize.y);	// 0, 0
	//	boundsGame->vertices[3].textureCoordinates = float2(ssCoordsX * ssSize.x + ssSize.x, ssCoordsY * ssSize.y);	// 1, 0
	//}

	if (selectedTexture != nullptr)
	{
		// Calculate the size of each sprite cell in the sprite sheet
		float spriteWidth = selectedTexture->GetWidth() / ssColumns;
		float spriteHeight = selectedTexture->GetHeight() / ssRows;

		// Calculate the texture coordinates of the sprite within the sprite sheet
		float minX = ssCoordsX * spriteWidth / selectedTexture->GetWidth();
		float minY = ssCoordsY * spriteHeight / selectedTexture->GetHeight();
		float maxX = (ssCoordsX + 1) * spriteWidth / selectedTexture->GetWidth();
		float maxY = (ssCoordsY + 1) * spriteHeight / selectedTexture->GetHeight();

		// Set the texture coordinates for each vertex of the quad
		boundsGame->vertices[0].textureCoordinates = float2(minX, maxY); // Bottom-left
		boundsGame->vertices[1].textureCoordinates = float2(maxX, maxY); // Bottom-right
		boundsGame->vertices[2].textureCoordinates = float2(minX, minY); // Top-left
		boundsGame->vertices[3].textureCoordinates = float2(maxX, minY); // Top-right
	}
}