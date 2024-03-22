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
				DirectionalLight* directionalLight = static_cast<DirectionalLight*>(lightReference);

				if (directionalLight) {

					ImGui::SeparatorText("DIRECTIONAL LIGHT");

					ImGui::Spacing();

					ImGui::SeparatorText("General Light Settings");

					ImGui::Spacing();

					ImGui::Checkbox("Show Debug", &directionalLight->debug);

					ImGui::Spacing();

					float3 color = directionalLight->GetColor();
					ImGui::ColorEdit3("Light Color", (float*)&color);
					directionalLight->SetColor(color);

					ImGui::Spacing();

					float intensity = directionalLight->GetIntensity();
					ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f, 100.0f);
					directionalLight->SetIntensity(intensity);

					ImGui::Spacing();

				}

				break;
			}
			case LightType::SPOT_LIGHT:
			{
				SpotLight* spotLight = static_cast<SpotLight*>(lightReference);

				if (spotLight) {

					ImGui::SeparatorText("SPOT LIGHT");

					ImGui::Spacing();

					ImGui::SeparatorText("General Light Settings");

					ImGui::Spacing();

					ImGui::Checkbox("Show Debug", &spotLight->debug);

					ImGui::Spacing();

					float3 color = spotLight->GetColor();
					ImGui::ColorEdit3("Light Color", (float*)&color);
					spotLight->SetColor(color);

					ImGui::Spacing();

					float intensity = spotLight->GetIntensity();
					ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f, 100.0f);
					spotLight->SetIntensity(intensity);

					ImGui::Spacing();

					ImGui::SeparatorText("Spot Light Settings");

					ImGui::Spacing();

					float range = spotLight->GetRange();
					ImGui::DragFloat("Range", &range, 0.1f, 0.0f, 100.0f);
					spotLight->SetRange(range);

					ImGui::Spacing();

					float radius = spotLight->GetRadius();
					ImGui::DragFloat("Radius", &radius, 0.1f, 0.0f, 100.0f);
					spotLight->SetRadius(radius);

					ImGui::Spacing();

				}

				break;
			}
			case LightType::AREA_LIGHT:
			{
				AreaLight* areaLight = static_cast<AreaLight*>(lightReference);

				if (areaLight) {

					ImGui::SeparatorText("AREA LIGHT");

					ImGui::Spacing();

					ImGui::SeparatorText("General Light Settings");

					ImGui::Spacing();

					ImGui::Checkbox("Show Debug", &areaLight->debug);

					ImGui::Spacing();

					float3 color = areaLight->GetColor();
					ImGui::ColorEdit3("Light Color", (float*)&color);
					areaLight->SetColor(color);

					ImGui::Spacing();

					float intensity = areaLight->GetIntensity();
					ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f, 100.0f);
					areaLight->SetIntensity(intensity);

					ImGui::Spacing();

					ImGui::SeparatorText("Area Light Settings");

					ImGui::Spacing();

					float range = areaLight->GetRange();
					ImGui::DragFloat("Range", &range, 0.1f, 0.0f, 100.0f);
					areaLight->SetRange(range);

					ImGui::Spacing();

					float width = areaLight->GetWidth();
					ImGui::DragFloat("Width", &width, 0.1f, 0.0f, 100.0f);
					areaLight->SetWidth(width);

					ImGui::Spacing();
					
					float height = areaLight->GetHeight();
					ImGui::DragFloat("Height", &height, 0.1f, 0.0f, 100.0f);
					areaLight->SetHeight(height);

					ImGui::Spacing();

				}

				break;
			}

		}

		// ----------------- End Content ----------------- 

		if (!active) { ImGui::EndDisabled(); }

		ImGui::Unindent();
	}

}