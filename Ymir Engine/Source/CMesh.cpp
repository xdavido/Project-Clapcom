#include "CMesh.h"

#include "External/ImGui/imgui.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"
#include "External/ImGui/backends/imgui_impl_opengl3.h"

#include "Application.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "ModuleResourceManager.h"

#include "External/mmgr/mmgr.h"

CMesh::CMesh(GameObject* owner) : Component(owner, ComponentType::MESH)
{
	meshReference = nullptr;
	rMeshReference = nullptr;
}

CMesh::~CMesh()
{
	External->resourceManager->UnloadResource(rMeshReference->GetUID());
	rMeshReference = nullptr;
}

void CMesh::Update()
{
	
}

void CMesh::OnInspector()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	bool exists = true;

	ImGui::Checkbox(("##" + std::to_string(UID)).c_str(), &active);
	ImGui::SameLine();

	if (ImGui::CollapsingHeader(("Mesh##" + std::to_string(UID)).c_str(), flags))
	{
		ImGui::Indent();

		ImGui::Spacing();

		if (!active) { ImGui::BeginDisabled(); }

		ImGui::Button("Drop .ymesh to change mesh", ImVec2(200, 50));
		YmeshDragDropTarget();

		ImGui::Spacing();

		ImGui::Text("UID: %d", rMeshReference->GetUID());

		ImGui::Spacing();

		ImGui::Spacing();

		ImGui::Text("Vertices: %d", nVertices);

		ImGui::Spacing();

		ImGui::Text("Indices: %d", nIndices);

		ImGui::Spacing();

		//for (auto it = External->renderer3D->models.begin(); it != External->renderer3D->models.end(); ++it) {

		//	for (auto jt = (*it).meshes.begin(); jt != (*it).meshes.end(); ++jt) {

		//		if ((*jt).meshGO->selected || (*it).modelGO->selected) {

		//			ImGui::Checkbox("Show Vertex Normals", &(*jt).enableVertexNormals);
		//			ImGui::Spacing();

		//			ImGui::Checkbox("Show Face Normals", &(*jt).enableFaceNormals);
		//			ImGui::Spacing();

		//		}

		//	}

		//}

		if (!active) { ImGui::EndDisabled(); }

		ImGui::Unindent();
	}
}

void CMesh::YmeshDragDropTarget()
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ymesh"))
		{
			std::string* libraryFilePathDrop = (std::string*)payload->Data;

			if (meshReference != nullptr) {

				//External->resourceManager->UnloadResource(meshReference->GetUID());

				meshReference = nullptr;

			}

			// Retrieve name of the file dropped, and then get the UID.

			// Lastly, Request Resource of the Mesh with given UID, Path and Type and render it (add a reference).

		}

		ImGui::EndDragDropTarget();
	}
}

void CMesh::InitBoundingBoxes()
{
	obb.SetNegativeInfinity();
	globalAABB.SetNegativeInfinity();

	std::vector<float3> floatArray;

	floatArray.reserve(rMeshReference->vertices.size());

	for (const auto& vertex : rMeshReference->vertices) {

		floatArray.push_back(vertex.position);

	}

	aabb.SetFrom(&floatArray[0], floatArray.size());
}

void CMesh::UpdateBoundingBoxes()
{
	obb = aabb;

	globalAABB.SetNegativeInfinity();
	globalAABB.Enclose(obb);
}

void CMesh::RenderBoundingBoxes()
{
	float3 verticesOBB[8];
	obb.GetCornerPoints(verticesOBB);
	External->renderer3D->DrawBox(verticesOBB, float3(255, 0, 0));

	float3 verticesAABB[8];
	globalAABB.GetCornerPoints(verticesAABB);
	External->renderer3D->DrawBox(verticesAABB, float3(0, 0, 255));
}
