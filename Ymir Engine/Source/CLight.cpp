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

}

void CLight::Update()
{
	switch (lightReference->GetType()) {

	case LightType::UNKNOWN:
	{
		

		break;
	}
	case LightType::POINT_LIGHT:
	{
		PointLight* pointLight = static_cast<PointLight*>(lightReference);

		if (pointLight) {

			pointLight->Update();
			pointLight->Render();

		}

		// Get Type

		// Change radius

		break;
	}
	case LightType::DIRECTIONAL_LIGHT:
	{
		

		break;
	}
	case LightType::SPOT_LIGHT:
	{
		

		break;
	}
	case LightType::AREA_LIGHT:
	{
		

		break;
	}

	}
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

			case LightType::UNKNOWN:
			{
				ImGui::Text("Unknown");

				break;
			}
			case LightType::POINT_LIGHT:
			{
				ImGui::Text("Point Light");

				PointLight* pointLight = static_cast<PointLight*>(lightReference);
				
				if (pointLight) {

					// Access PointLight's specific members
					float radius = pointLight->GetRadius();
					ImGui::SliderFloat("Radius", &radius, 0.0f, 100.0f);
					pointLight->SetRadius(radius); // If you want to set the new radius back to the PointLight

					float intensity = pointLight->GetIntensity();
					ImGui::SliderFloat("Intensity", &intensity, 0.0f, 100.0f);
					pointLight->SetIntensity(intensity); // If you want to set the new radius back to the PointLight

					float3 color = pointLight->GetColor();

				}

				// Get Type

				// Change radius

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