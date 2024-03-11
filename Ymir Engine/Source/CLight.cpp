#include "CLight.h"

#include "External/ImGui/imgui.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"
#include "External/ImGui/backends/imgui_impl_opengl3.h"

#include <string>

CLight::CLight(GameObject* owner) : Component(owner, ComponentType::LIGHT)
{
	lightOwner = static_cast<Light*>(owner);
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

		switch (lightOwner->GetType()) {

			case LightType::UNKNOWN:
			{
				ImGui::Text("Unknown");

				break;
			}
			case LightType::POINT_LIGHT:
			{
				ImGui::Text("Point Light");

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