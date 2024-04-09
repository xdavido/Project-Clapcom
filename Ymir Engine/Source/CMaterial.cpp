#include "CMaterial.h"

#include "External/ImGui/imgui.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"
#include "External/ImGui/backends/imgui_impl_opengl3.h"

#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleResourceManager.h"
#include "GameObject.h"

#include "ShaderEditor.h"
#include "PhysfsEncapsule.h"

#include "External/mmgr/mmgr.h"

CMaterial::CMaterial(GameObject* owner) : Component(owner, ComponentType::MATERIAL)
{
    diffuse_UID = 0;
    diffuse_ID = 0;

    specular_UID = 0;
    specular_ID = 0;

    normal_UID = 0;
    normal_ID = 0;

    height_UID = 0;
    height_ID = 0;

    ambient_UID = 0;
    ambient_ID = 0;

    emissive_UID = 0;
    emissive_ID = 0;

    selectedShader = 0;

    this->owner = owner;

    // Load MyShader.glsl by default
    shaderPath = SHADER_VS_FS;
    shader.LoadShader(shaderPath);
    shaderDirtyFlag = false;
}

CMaterial::~CMaterial()
{
    shader.ClearShader();

    for (auto it = rTextures.rbegin(); it != rTextures.rend(); ++it)
    {
        External->resourceManager->UnloadResource((*it)->GetUID());
        (*it) = nullptr;
    }
}

void CMaterial::Update()
{

}

void CMaterial::OnInspector()
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

    bool exists = true;

    ImGui::Checkbox(("##" + std::to_string(UID)).c_str(), &active);
    ImGui::SameLine();

    if (ImGui::CollapsingHeader(("Material##" + std::to_string(UID)).c_str(), &exists, flags))
    {
        ImGui::Indent();

        ImGui::Spacing();

        if (!active) { ImGui::BeginDisabled(); }

        // ------------------------------------ SHADER ------------------------------------

        ImGui::SeparatorText("SHADER");

        ImGui::Spacing();

        ImGui::Text("Shader: ");
        ImGui::SameLine();

        if (owner->selected) {

            // Find the index of the current shader path in listShaderPaths
            auto it = std::find(listShaderPaths.begin(), listShaderPaths.end(), shaderPath);

            if (it != listShaderPaths.end()) {

                selectedShader = static_cast<int>(std::distance(listShaderPaths.begin(), it));

            }

        }

        // Choose between the list of shaders
        if (ImGui::Combo("##ChooseShader", &selectedShader, listShaderNames.data(), listShaderNames.size())) {
            
            shaderDirtyFlag = true;

        }

        if (shaderDirtyFlag) {

            // When shader changes, update the shader path and recompile
            shaderPath = listShaderPaths[selectedShader];
            shader.LoadShader(shaderPath);

            // Reset the dirty flag after handling the change
            shaderDirtyFlag = false;

        }

        ImGui::Spacing();

        // Shader Uniforms Management

        if (owner->selected) {

            if (shader.uniforms.size() == 0) {

                ImGui::Text("No editable uniforms.");

            }
            else {

                ImGui::Text("Uniforms:");

            }

            ImGui::Spacing();

            ImGui::Indent();

            // In case the shader has editable uniforms:
            for (auto kt = shader.uniforms.begin(); kt != shader.uniforms.end(); ++kt) {

                std::string label = "##" + kt->name;

                ImGui::Text("%s", kt->name.c_str());
                ImGui::SameLine();

                // Change display according to uniform type
                switch (kt->type)
                {
                case UniformType::boolean:

                    ImGui::Checkbox(label.c_str(), (bool*)kt->value.get());

                    shader.SetUniformValue(kt->name, (bool*)kt->value.get());

                    break;

                case UniformType::i1:

                    ImGui::DragInt(label.c_str(), (int*)kt->value.get(), 0.1f);

                    shader.SetUniformValue(kt->name, (int*)kt->value.get());

                    break;

                case UniformType::i2:

                    ImGui::DragInt2(label.c_str(), (int*)kt->value.get(), 0.1f);

                    shader.SetUniformValue(kt->name, (int*)kt->value.get());

                    break;

                case UniformType::i3:

                    ImGui::DragInt3(label.c_str(), (int*)kt->value.get(), 0.1f);

                    shader.SetUniformValue(kt->name, (int*)kt->value.get());

                    break;

                case UniformType::i4:

                    ImGui::DragInt4(label.c_str(), (int*)kt->value.get(), 0.1f);

                    shader.SetUniformValue(kt->name, (int*)kt->value.get());

                    break;

                case UniformType::f1:

                    if (kt->name == "transparency")
                    {
                        ImGui::DragFloat(label.c_str(), (float*)kt->value.get(), 0.02f, 0.0f, 1.0f);
                    }
                    else 
                    {
                        ImGui::DragFloat(label.c_str(), (float*)kt->value.get(), 0.1f);
                    }

                    shader.SetUniformValue(kt->name, (float*)kt->value.get());

                    break;

                case UniformType::f2:

                    ImGui::DragFloat2(label.c_str(), (float*)kt->value.get(), 0.1f);

                    shader.SetUniformValue(kt->name, (float*)kt->value.get());

                    break;

                case UniformType::f3:

                    ImGui::DragFloat3(label.c_str(), (float*)kt->value.get(), 0.1f);

                    shader.SetUniformValue(kt->name, (float*)kt->value.get());

                    break;

                case UniformType::f4:

                    ImGui::DragFloat4(label.c_str(), (float*)kt->value.get(), 0.1f);

                    shader.SetUniformValue(kt->name, (float*)kt->value.get());

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

        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(diffuse_ID)), textureMapSize);
        DdsDragDropTarget();
        ImGui::SameLine();
        ImGui::Text("Diffuse");
        ImGui::SameLine();

        std::string diffuseName;
        PhysfsEncapsule::SplitFilePath(diffuse_path.c_str(), nullptr, &diffuseName);
        ImGui::Text("(%s)", diffuseName.c_str());

        ImGui::Spacing();

        //ImGui::ColorButton("Specular", ImVec4(0, 0, 0, 0), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, textureMapSize);

        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(specular_ID)), textureMapSize);
        DdsDragDropTarget();
        ImGui::SameLine();
        ImGui::Text("Specular");
        ImGui::SameLine();
        
        std::string specularName;
        PhysfsEncapsule::SplitFilePath(specular_path.c_str(), nullptr, &specularName);
        ImGui::Text("(%s)", specularName.c_str());

        ImGui::Spacing();

        //ImGui::ColorButton("Normal", ImVec4(0, 0, 0, 0), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, textureMapSize);

        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(normal_ID)), textureMapSize);
        DdsDragDropTarget();
        ImGui::SameLine();
        ImGui::Text("Normal");
        ImGui::SameLine();
        
        std::string normalName;
        PhysfsEncapsule::SplitFilePath(normal_path.c_str(), nullptr, &normalName);
        ImGui::Text("(%s)", normalName.c_str());

        ImGui::Spacing();

        //ImGui::ColorButton("Height", ImVec4(0, 0, 0, 0), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, textureMapSize);

        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(height_ID)), textureMapSize);
        DdsDragDropTarget();
        ImGui::SameLine();
        ImGui::Text("Height");
        ImGui::SameLine();
        
        std::string heightName;
        PhysfsEncapsule::SplitFilePath(height_path.c_str(), nullptr, &heightName);
        ImGui::Text("(%s)", heightName.c_str());

        ImGui::Spacing();

        //ImGui::ColorButton("Ambient", ImVec4(0, 0, 0, 0), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, textureMapSize);

        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(ambient_ID)), textureMapSize);
        DdsDragDropTarget();
        ImGui::SameLine();
        ImGui::Text("Ambient");
        ImGui::SameLine();
        
        std::string ambientName;
        PhysfsEncapsule::SplitFilePath(ambient_path.c_str(), nullptr, &ambientName);
        ImGui::Text("(%s)", ambientName.c_str());

        ImGui::Spacing();

        //ImGui::ColorButton("Emissive", ImVec4(0, 0, 0, 0), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, textureMapSize);

        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(emissive_ID)), textureMapSize);
        DdsDragDropTarget();
        ImGui::SameLine();
        ImGui::Text("Emissive");
        ImGui::SameLine();
        
        std::string emissiveName;
        PhysfsEncapsule::SplitFilePath(emissive_path.c_str(), nullptr, &emissiveName);
        ImGui::Text("(%s)", emissiveName.c_str());

        ImGui::Spacing();

        // Utility buttons

        if (ImGui::Button("Apply Checker Texture")) {

            External->renderer3D->ApplyCheckerTexture();

        }

        ImGui::Spacing();

        if (ImGui::Button("Clear All Textures")) {

            for (auto it = rTextures.begin(); it != rTextures.end(); ++it)
            {
                External->resourceManager->UnloadResource((*it)->GetUID());
                
            }

            rTextures.clear();

        }

        if (!active) { ImGui::EndDisabled(); }

        ImGui::Spacing();

        ImGui::Unindent();
    }
    
    if (!exists) { mOwner->RemoveComponent(this); }
}

void CMaterial::DdsDragDropTarget()
{
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("tex"))
        {
            std::string* libraryFilePathDrop = (std::string*)payload->Data;

            // Unload Resource of the previous texture loaded if it had any.

            // Retrieve name of the file dropped, and then get the UID.

            // Lastly, Request Resource of the Texture with given UID, Path and Type and render it (add a reference).

        }

        ImGui::EndDragDropTarget();
    }
}
    