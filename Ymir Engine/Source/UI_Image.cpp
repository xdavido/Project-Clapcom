#include "UI_Image.h"
#include "GameObject.h"

#include "Application.h"
#include "ModuleEditor.h"
#include "ImporterTexture.h"

//#include "External/ImGui/imgui_custom.h"

#include "External/mmgr/mmgr.h"

UI_Image::UI_Image(GameObject* g, float x, float y, float w, float h, std::string shaderPath, std::string imgPath) : C_UI(UI_TYPE::IMAGE, ComponentType::UI, g, "Image", x, y, w, h)
{
    mat = new CMaterial(g);
    mat->shaderPath = shaderPath;
    mat->shader.LoadShader(mat->shaderPath);

    ResourceTexture* rTexTemp = new ResourceTexture();
    ImporterTexture::Import(imgPath, rTexTemp);
    rTexTemp->type = TextureType::DIFFUSE;
    rTexTemp->UID = Random::Generate();
    mat->path = imgPath;
    mat->rTextures.push_back(rTexTemp);
}

UI_Image::~UI_Image()
{
	RELEASE(mat);
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

        if (ImGui::Button("Set Native Size", ImVec2(110, 30)))
        {
            SetNativeSize();
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
//	if (mat != nullptr && mat->tex != nullptr)
//	{
		UI_Bounds* boundsDrawn = nullptr;

		if (game)
		{
			boundsDrawn = boundsGame;
		}

		else
		{
			boundsDrawn = boundsEditor;

			glPushMatrix();
			glMultMatrixf(mOwner->mTransform->mGlobalMatrix.Transposed().ptr());
		}

		// Render
		glBindVertexArray(boundsDrawn->VAO);

		glDrawElements(GL_TRIANGLES, boundsDrawn->indices.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		//mat->shader.UseShader(false);

		//for (auto& textures : mat->rTextures) {

		//	textures->BindTexture(false);

		//}

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
    width = mat->rTextures[0]->GetWidth();
    height = mat->rTextures[0]->GetHeight();
}
