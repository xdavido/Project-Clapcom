#include "CLight.h"

#include "External/ImGui/imgui.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"
#include "External/ImGui/backends/imgui_impl_opengl3.h"

#include <string>

CLight::CLight(GameObject* owner, LightType type) : Component(owner, ComponentType::LIGHT)
{
	switch (type) {
		case LightType::UNKNOWN:
			lightOwner = dynamic_cast<Light*>(owner);
			break;
		case LightType::POINT_LIGHT:
			lightOwner = dynamic_cast<PointLight*>(owner);
			break;
		case LightType::DIRECTIONAL_LIGHT:
			lightOwner = dynamic_cast<DirectionalLight*>(owner);
			break;
		case LightType::SPOT_LIGHT:
			lightOwner = dynamic_cast<SpotLight*>(owner);
			break;
		case LightType::AREA_LIGHT:
			lightOwner = dynamic_cast<AreaLight*>(owner);
			break;
	}

	this->type = type;
	
}

CLight::~CLight()
{
	RELEASE(lightOwner);
}

void CLight::Update()
{

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

	
			switch (type) {

			case LightType::UNKNOWN:
			{
				ImGui::Text("Unknown");

				break;
			}
			case LightType::POINT_LIGHT:
			{
				ImGui::Text("Point Light");

				PointLight* pointLight = new PointLight();
				pointLight = dynamic_cast<PointLight*>(lightOwner);

				if (pointLight) {

					// Access PointLight's specific members
					float radius = pointLight->GetRadius();
					ImGui::SliderFloat("Radius", &radius, 0.0f, 100.0f);
					pointLight->SetRadius(radius); // If you want to set the new radius back to the PointLight
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