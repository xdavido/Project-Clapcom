#include "UI_Canvas.h"
#include "ModuleScene.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"

#include "External/mmgr/mmgr.h"

UI_Canvas::UI_Canvas(GameObject* g, float w, float h) : C_UI(UI_TYPE::CANVAS, ComponentType::UI, g, "Canvas")
{
	static_cast<G_UI*>(mOwner)->canvas = this;
	External->scene->SetCanvas((G_UI*)mOwner);
	External->scene->vCanvas.push_back((G_UI*)mOwner);

	tabNav_ = false;
}

UI_Canvas::~UI_Canvas()
{
	if (External->scene->GetCanvas() == mOwner)
	{
		External->scene->SetCanvas();
	}

	External->scene->vCanvas.erase(std::find(External->scene->vCanvas.begin(), External->scene->vCanvas.end(), (G_UI*)mOwner));
	External->scene->vCanvas.shrink_to_fit();
}

void UI_Canvas::OnInspector()
{
	bool exists = true;
	ImGui::Checkbox(("##UI Canvas Checkbox" + std::to_string(mOwner->UID)).c_str(), &active);		ImGui::SameLine();

	if (ImGui::CollapsingHeader(("Canvas##UI Collapsing Header" + std::to_string(mOwner->UID)).c_str(), &exists, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (!active) { ImGui::BeginDisabled(); }

		ImGui::Text("Canvas");

		if (!active) { ImGui::EndDisabled(); }
	}
	ImGui::SameLine();

	if (!exists) { mOwner->RemoveComponent(this); }
}

void UI_Canvas::Draw(bool game)
{
	if (!game)
	{
		glBegin(GL_LINE_LOOP);
		glColor4f(0.0f, 1.0f, 1.0f, 1.0f);

		float3 v1 = float3(posX, posY, 0);
		float3 v2 = float3(posX + width, posY, 0);
		float3 v3 = float3(posX, posY + height, 0);
		float3 v4 = float3(posX + width, posY + height, 0);

		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v4.x, v4.y, v4.z);
		glVertex3f(v3.x, v3.y, v3.z);

		glEnd();
	}
}

void UI_Canvas::UpdateUITransform()
{
	mOwner->mTransform->SetTransform(float3(0, 0, 0), float3(0, 0, 0), float3(1, 1, 1));
	
#ifndef _STANDALONE

	width = External->editor->gameViewSize.x;
	height = External->editor->gameViewSize.y; 

#else

	width = External->window->width;
	height = External->window->height;

#endif // !_STANDALONE

	boundsEditor->vertices[0].position = float3(posX, posY, 0);
	boundsEditor->vertices[1].position = float3(posX + width, posY, 0);
	boundsEditor->vertices[2].position = float3(posX, posY + height, 0);
	boundsEditor->vertices[3].position = float3(posX + width, posY + height, 0);

	boundsEditor->vertices[2].textureCoordinates = float2(0, 1);
	boundsEditor->vertices[3].textureCoordinates = float2(1, 1);
	boundsEditor->vertices[1].textureCoordinates = float2(1, 0);
	boundsEditor->vertices[0].textureCoordinates = float2(0, 0);

	dirty_ = false;

	boundsEditor->RegenerateVBO();
}

