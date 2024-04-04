#include "CNavMeshAgent.h"
#include "ImGui/imgui.h"
#include "Application.h"

CNavMeshAgent::CNavMeshAgent(GameObject* owner) : Component(owner, ComponentType::NAVMESHAGENT)
{

}

CNavMeshAgent::~CNavMeshAgent()
{
	selectedNav = nullptr;
	path.clear();
}



void CNavMeshAgent::OnInspector()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	bool exists = true;

	ImGui::Checkbox(("##" + std::to_string(UID)).c_str(), &active);
	ImGui::SameLine();

	if (ImGui::CollapsingHeader(("NavMesh Agent##" + std::to_string(UID)).c_str(), &exists, flags)) {
		ImGui::Columns(2, NULL, false);
		ImGui::Dummy({ 0,17 });
		ImGui::Text("Speed");
		ImGui::Spacing();
		ImGui::Text("Angular Speed");
		ImGui::Spacing();
		ImGui::Text("Stopping Distance");
		ImGui::Spacing();
		ImGui::NextColumn();

		char buffer[50];

		ImGui::Dummy({ 0,17 });
		sprintf_s(buffer, 50, "%.2f", properties.speed);
		if (ImGui::InputText("##speed", &buffer[0], sizeof(buffer)))
		{
			if (buffer[0] != '\0') {
				properties.speed = strtod(buffer, NULL);
			}
		}

		sprintf_s(buffer, 50, "%.2f", properties.angularSpeed);
		if (ImGui::InputText("##angularSpeed", &buffer[0], sizeof(buffer)))
		{
			if (buffer[0] != '\0') {
				properties.angularSpeed = strtod(buffer, NULL);
			}
		}

		sprintf_s(buffer, 50, "%.2f", properties.stoppingDistance);
		if (ImGui::InputText("##stoppingDistance", &buffer[0], sizeof(buffer)))
		{
			if (buffer[0] != '\0') {
				properties.stoppingDistance = strtod(buffer, NULL);
			}
		}


		ImGui::NextColumn();
		ImGui::Columns(1);
	}
	
}
