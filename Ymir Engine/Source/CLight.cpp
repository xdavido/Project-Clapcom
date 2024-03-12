#include "CLight.h"

#include "External/ImGui/imgui.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"
#include "External/ImGui/backends/imgui_impl_opengl3.h"

#include <string>

CLight::CLight(GameObject* owner, Light* lightReference) : Component(owner, ComponentType::LIGHT)
{
	this->lightReference = lightReference;
}

CLight::~CLight()
{
	auto it = std::find(External->lightManager->lights.begin(), External->lightManager->lights.end(), lightReference);

	if (it != External->lightManager->lights.end()) {

		External->lightManager->lights.erase(it);

	}

	delete lightReference;

}

void CLight::Update()
{
	lightReference->Update();
}

void CLight::OnInspector()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	bool exists = true;

	ImGui::Checkbox(("##" + std::to_string(UID)).c_str(), &active);
	ImGui::SameLine();

	if (ImGui::CollapsingHeader(("Light##" + std::to_string(UID)).c_str(), flags))
	{
		ImGui::Indent();

		ImGui::Spacing();

		if (!active) { ImGui::BeginDisabled(); }

		// ----------------- Content ----------------- 

		switch (lightReference->GetType()) {

			case LightType::POINT_LIGHT:
			{
				PointLight* pointLight = static_cast<PointLight*>(lightReference);
				
				if (pointLight) {

					ImGui::SeparatorText("POINT LIGHT");

					ImGui::Spacing();

					ImGui::SeparatorText("General Light Settings");

					ImGui::Spacing();

					ImGui::Checkbox("Show Debug", &pointLight->debug);

					ImGui::Spacing();

					float3 color = pointLight->GetColor();
					ImGui::ColorEdit3("Light Color", (float*)&color);
					pointLight->SetColor(color);

					ImGui::Spacing();

					float intensity = pointLight->GetIntensity();
					ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f, 100.0f);
					pointLight->SetIntensity(intensity);

					ImGui::Spacing();

					ImGui::SeparatorText("Point Light Settings");

					ImGui::Spacing();

					float radius = pointLight->GetRadius();
					ImGui::DragFloat("Radius", &radius, 0.1f, 0.0f, 100.0f);
					pointLight->SetRadius(radius); 

					ImGui::Spacing();

				}

				break;
			}
			case LightType::DIRECTIONAL_LIGHT:
			{
				ImGui::Text("Directional Light");

				break;
			}
			case LightType::SPOT_LIGHT:
			{
				ImGui::Text("Spot Light");

				break;
			}
			case LightType::AREA_LIGHT:
			{
				ImGui::Text("Area Light");

				break;
			}

		}

		// ----------------- End Content ----------------- 

		if (!active) { ImGui::EndDisabled(); }

		ImGui::Unindent();
	}

}