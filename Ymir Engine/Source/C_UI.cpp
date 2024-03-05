#include "C_UI.h"

#include "UI_Canvas.h"

#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleInput.h"

#include "External/mmgr/mmgr.h"

C_UI::C_UI(UI_TYPE ui_t, ComponentType t, GameObject* g, std::string n, int x, int y, int w, int h, Color c) : Component(g, t)
{
	UI_type = ui_t;
	state = UI_STATE::NORMAL;

	width = w;
	height = h;
	color = c;

	isDragging = false;
	isDraggable = false;
	fade = false;

	mOwner->mTransform->SetPosition(float3(x, y, 0));
	mOwner->mTransform->UpdateGlobalMatrix();

	float3 position = mOwner->mTransform->translation;

	float3 globalPos;
	Quat rot;
	float3 scale;
	mOwner->mTransform->mGlobalMatrix.Decompose(globalPos, rot, scale);

	if (x != 0 || y != 0)
	{
		posX = x;
		posY = y;
	}
	else
	{
		posX = globalPos.x;
		posY = globalPos.y;
	}

	scaleBounds = scale;

	boundsEditor = new UI_Bounds;
	boundsGame = new UI_Bounds;

	GLuint indexE0 = 0;
	GLuint indexE1 = 1;
	GLuint indexE2 = 2;
	GLuint indexE3 = 2;
	GLuint indexE4 = 1;
	GLuint indexE5 = 3;

	boundsEditor->indices.push_back(indexE0);
	boundsEditor->indices.push_back(indexE1);
	boundsEditor->indices.push_back(indexE2);
	boundsEditor->indices.push_back(indexE3);
	boundsEditor->indices.push_back(indexE4);
	boundsEditor->indices.push_back(indexE5);

	GLuint indexG0 = 0;
	GLuint indexG1 = 1;
	GLuint indexG2 = 2;
	GLuint indexG3 = 2;
	GLuint indexG4 = 1;
	GLuint indexG5 = 3;

	boundsGame->indices.push_back(indexG0);
	boundsGame->indices.push_back(indexG1);
	boundsGame->indices.push_back(indexG2);
	boundsGame->indices.push_back(indexG3);
	boundsGame->indices.push_back(indexG4);
	boundsGame->indices.push_back(indexG5);

	Vertex editorVertex1;
	editorVertex1.position = float3(position.x, position.y + (height * scaleBounds.y), 0);
	editorVertex1.normal = float3(0,0,0);
	editorVertex1.textureCoordinates = float2(0, 1);
	boundsEditor->vertices.push_back(editorVertex1);

	Vertex editorVertex2;
	editorVertex2.position = float3(position.x + (width * scaleBounds.x), position.y + ((height * scaleBounds.y) * scaleBounds.y), 0);
	editorVertex2.normal = float3(0, 0, 0);
	editorVertex2.textureCoordinates = float2(1, 1);
	boundsEditor->vertices.push_back(editorVertex2);

	Vertex editorVertex3;
	editorVertex3.position = float3(position.x, position.y, 0);
	editorVertex3.normal = float3(0, 0, 0);
	editorVertex3.textureCoordinates = float2(0, 0);
	boundsEditor->vertices.push_back(editorVertex3);

	Vertex editorVertex4;
	editorVertex4.position = float3(position.x + (width * scaleBounds.x), position.y, 0);
	editorVertex4.normal = float3(0, 0, 0);
	editorVertex4.textureCoordinates = float2(1, 0);
	boundsEditor->vertices.push_back(editorVertex4);

	Vertex gameVertex1;
	gameVertex1.position = float3(posX, posY + (height * scaleBounds.y), 0);
	gameVertex1.normal = float3(0, 0, 0);
	gameVertex1.textureCoordinates = float2(0, 1);
	boundsGame->vertices.push_back(gameVertex1);

	Vertex gameVertex2;
	gameVertex2.position = float3(posX + (width * scaleBounds.x), posY + (height * scaleBounds.y), 0);
	gameVertex2.normal = float3(0, 0, 0);
	gameVertex2.textureCoordinates = float2(1, 1);
	boundsGame->vertices.push_back(gameVertex2);

	Vertex gameVertex3;
	gameVertex3.position = float3(posX, posY, 0);
	gameVertex3.normal = float3(0, 0, 0);
	gameVertex3.textureCoordinates = float2(0, 0);
	boundsGame->vertices.push_back(gameVertex3);

	Vertex gameVertex4;
	gameVertex4.position = float3(posX + (width * scaleBounds.x), posY, 0);
	gameVertex4.normal = float3(0, 0, 0);
	gameVertex4.textureCoordinates = float2(1, 0);
	boundsGame->vertices.push_back(gameVertex4);

	boundsEditor->InitBuffers();
	boundsGame->InitBuffers();

	//Mouse pick
	local_aabb.SetNegativeInfinity();
	//local_aabb.Enclose((float3*)boundsEditor->vertex, 4);
	obb = local_aabb;
	obb.Transform(mOwner->mTransform->GetGlobalTransform());
	global_aabb.SetNegativeInfinity();
	global_aabb.Enclose(obb);
}

C_UI::~C_UI()
{
	boundsEditor->DeleteBuffers();
	boundsGame->DeleteBuffers();
	//RELEASE_ARRAY(boundsEditor->index);
	//RELEASE_ARRAY(boundsGame->index);
	RELEASE(boundsEditor);
	RELEASE(boundsGame);
}

update_status C_UI::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return ret;
}

void C_UI::Draw(bool game)
{
	UI_Bounds* boundsDrawn = nullptr;

	if (game)
	{
		boundsDrawn = boundsGame;

		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//glOrtho(0.0, External->editor->gameViewSize.x, 0.0, External->editor->gameViewSize.y, 1.0, -1.0);

		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
	}

	else
	{
		boundsDrawn = boundsEditor;

		glPushMatrix();
		glMultMatrixf(mOwner->mTransform->mGlobalMatrix.Transposed().ptr());
	}

	glBindVertexArray(boundsDrawn->VAO);

	glDrawElements(GL_TRIANGLES, boundsDrawn->indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	
	if (!game)
	{
		glPopMatrix();
	}

	boundsDrawn = nullptr;
}

void C_UI::DebugDraw()
{
	glBegin(GL_LINE_LOOP);
	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);

	float3 position = mOwner->mTransform->translation;

	float3 v1 = float3(position.x, position.y, position.z);
	float3 v2 = float3(position.x + (width * scaleBounds.x), position.y, position.z);
	float3 v3 = float3(position.x, position.y + (height * scaleBounds.y), position.z);
	float3 v4 = float3(position.x + (width * scaleBounds.x), position.y + (height * scaleBounds.y), position.z);

	glVertex3f(v1.x, v1.y, v1.z);
	glVertex3f(v2.x, v2.y, v2.z);
	glVertex3f(v4.x, v4.y, v4.z);
	glVertex3f(v3.x, v3.y, v3.z);

	glEnd();
}

void C_UI::StateLogic()
{
	if (External->editor->g->HoveredWindow != nullptr)
	{
		std::string name = External->editor->g->HoveredWindow->Name;
		if (name == "Game" && !static_cast<G_UI*>(mOwner)->canvas->fade)
		{
			float2 mousePos = float2(External->editor->mouse.x, External->editor->mouse.y);

			switch (state)
			{
			case UI_STATE::DISABLED:
				break;
			case UI_STATE::NORMAL:
			{
				OnNormal();
				if (MouseCheck(mousePos))
				{
					state = UI_STATE::FOCUSED;
				}
			}
				break;
			case UI_STATE::FOCUSED:	// On hover
			{
				OnFocused();
				if ((External->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) ||
					(External->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !ImGui::GetIO().WantTextInput && !External->input->GetInputActive()))
				{
					state = UI_STATE::PRESSED;
				}
				if (!MouseCheck(mousePos))
				{
					state = UI_STATE::NORMAL;
				}
			}
				break;
			case UI_STATE::PRESSED: // On hold
			{
				OnPressed();
				if ((External->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && MouseCheck(mousePos)) ||
					(External->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP && !ImGui::GetIO().WantTextInput && !External->input->GetInputActive()))
				{
					state = UI_STATE::RELEASE;
				}
				else if (External->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && !MouseCheck(mousePos))
				{
					state = UI_STATE::NORMAL;
				}
			}
				break;
			case UI_STATE::RELEASE:
			{
				OnRelease();
				state = UI_STATE::SELECTED;
			}
				break;
			case UI_STATE::SELECTED:
			{
				OnSelected();
				if (External->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && MouseCheck(mousePos) ||
					(External->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !ImGui::GetIO().WantTextInput && !External->input->GetInputActive()))
				{
					state = UI_STATE::PRESSED;
				}
				else if (External->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && !MouseCheck(mousePos))
				{
					state = UI_STATE::NORMAL;
				}
			}
				break;
			case UI_STATE::NONE:
				break;
			default:
				break;
			}
		}
	}
}

void C_UI::OnNormal()
{
}

void C_UI::OnFocused()
{
}

void C_UI::OnPressed()
{
	if (isDraggable)
	{
		isDragging = true;
	}
}

void C_UI::OnRelease()
{
	isDragging = false;
}

void C_UI::OnSelected()
{
}

void C_UI::SetState(UI_STATE uiState)
{
	state = uiState;
}

bool C_UI::MouseCheck(float2 mouse)
{
	mouse.y = (External->editor->gameViewSize.y - (mouse.y * External->editor->gameViewSize.y)) / External->editor->gameViewSize.y;

	return (mouse.x >= posX / External->editor->gameViewSize.x && mouse.x <= (posX + (width * scaleBounds.x)) / External->editor->gameViewSize.x
		&& mouse.y >= (posY - 20) / External->editor->gameViewSize.y && mouse.y <= (posY - 20 + (height * scaleBounds.y)) / External->editor->gameViewSize.y);
}

void C_UI::UpdateUITransform()
{
	float3 position = mOwner->mTransform->translation;
	float3 globalPos;
	Quat rot;
	float3 scale;

	mOwner->mTransform->mGlobalMatrix.Decompose(globalPos, rot, scale);

	posX = globalPos.x;//TODO: arreglar
	posY = globalPos.y;
	scaleBounds = scale;

	boundsEditor->vertices[0].position = float3(position.x, position.y + ((height * scaleBounds.y) * scale.y), 0);
	boundsEditor->vertices[1].position = float3(position.x + (width * scaleBounds.x), position.y + ((height * scaleBounds.y) * scale.y), 0);
	boundsEditor->vertices[2].position = float3(position.x, position.y, 0);
	boundsEditor->vertices[3].position = float3(position.x + (width * scaleBounds.x), position.y, 0);

	boundsGame->vertices[0].position = float3(posX, posY + (height * scaleBounds.y), 0);
	boundsGame->vertices[1].position = float3(posX + (width * scaleBounds.x), posY + (height * scaleBounds.y), 0);
	boundsGame->vertices[2].position = float3(posX, posY, 0);
	boundsGame->vertices[3].position = float3(posX + (width * scaleBounds.x), posY, 0);

	boundsEditor->RegenerateVBO();
	boundsGame->RegenerateVBO();

	/*if (!gameObject->vChildren.empty())
	{
		for (auto i = 0; i < gameObject->vChildren.size(); i++)
		{
			if (gameObject->vChildren[i]->GetComponentByType(C_TYPE::UI) != nullptr)
			{
				static_cast<C_UI*>(gameObject->vChildren[i]->GetComponentByType(C_TYPE::UI))->UpdateUITransform();
			}
		}
	}*/
}

void C_UI::DrawABB()
{
	glBegin(GL_LINES);
	glLineWidth(3.0f);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(global_aabb.Edge(i).a.x, global_aabb.Edge(i).a.y, global_aabb.Edge(i).a.z);
		glVertex3f(global_aabb.Edge(i).b.x, global_aabb.Edge(i).b.y, global_aabb.Edge(i).b.z);
	}

	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void C_UI::DrawOBB()
{
	glBegin(GL_LINES);
	glLineWidth(3.0f);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(obb.Edge(i).a.x, obb.Edge(i).a.y, obb.Edge(i).a.z);
		glVertex3f(obb.Edge(i).b.x, obb.Edge(i).b.y, obb.Edge(i).b.z);
	}

	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void C_UI::UpdateBoundingBoxes()
{
	local_aabb.SetNegativeInfinity();
	local_aabb.Enclose((float3*)&boundsEditor->vertices[0].position, 4);
	obb = local_aabb;
	obb.Transform(mOwner->mTransform->GetGlobalTransform());
	global_aabb.SetNegativeInfinity();
	global_aabb.Enclose(obb);
}

void C_UI::Drag(float dt)
{
	int movementX = External->input->GetMouseXMotion() * dt * 30;
	int movementY = -External->input->GetMouseYMotion() * dt * 30;
	posX += movementX;
	posY += movementY;


	float3 globalPos;
	Quat rot;
	float3 scale;
	//TODO:MILAGRO
	mOwner->mTransform->SetPosition(float3(mOwner->mTransform->translation.x + movementX, mOwner->mTransform->translation.y + movementY, 0));
	mOwner->mTransform->mGlobalMatrix.Decompose(globalPos, rot, scale);

	scaleBounds = scale;

	float3 position = mOwner->mTransform->translation;

	boundsEditor->vertices[0].position = float3(position.x + movementX, position.y + (height * scaleBounds.y) + movementY, 0);
	boundsEditor->vertices[1].position = float3(position.x + (width * scaleBounds.x) + movementX, position.y + (height * scaleBounds.y) + movementY, 0);
	boundsEditor->vertices[2].position = float3(position.x + movementX, position.y + movementY, 0);
	boundsEditor->vertices[3].position = float3(position.x + (width * scaleBounds.x) + movementX, position.y + movementY, 0);

	boundsGame->vertices[0].position = float3(posX, posY + (height * scaleBounds.y), 0);
	boundsGame->vertices[1].position = float3(posX + (width * scaleBounds.x), posY + (height * scaleBounds.y), 0);
	boundsGame->vertices[2].position = float3(posX, posY, 0);
	boundsGame->vertices[3].position = float3(posX + (width * scaleBounds.x), posY, 0);


	boundsEditor->RegenerateVBO();
	boundsGame->RegenerateVBO();

	//if (!gameObject->vChildren.empty())
	//{
	//	for (auto i = 0; i < gameObject->vChildren.size(); i++)
	//	{
	//		if (gameObject->vChildren[i]->GetComponentByType(C_TYPE::UI) != nullptr)
	//		{
	//			static_cast<C_UI*>(gameObject->vChildren[i]->GetComponentByType(C_TYPE::UI))->Drag(dt);
	//		}
	//	}
	//}
}

void C_UI::FadeUI(float dt)
{
	if (color.a <= 0)
	{
		mOwner->active = false;
		//External->scene->crossHair = true;
	}
	else
	{
		//color.a -= 0.1 * dt;
		color.a -= 1 * dt;
	}

	if (!mOwner->mChildren.empty())
	{
		for (auto i = 0; i < mOwner->mChildren.size(); i++)
		{
			if (mOwner->mChildren[i]->GetComponent(ComponentType::UI) != nullptr)
			{
				static_cast<C_UI*>(mOwner->mChildren[i]->GetComponent(ComponentType::UI))->FadeUI(dt);
			}
		}
	}
}

bool UI_Bounds::InitBuffers()
{
	VBO = 0;
	EBO = 0;
	VAO = 0;
	id_tex_uvs = 0;

	// 1. Create Buffers

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//glGenBuffers(1, &id_tex_uvs);


	if (VBO == 0 || EBO == 0 || VAO == 0)
	{
		LOG("[ERROR] buffer not created");
		return false;
	}

	// 2. Bind Buffers
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBindBuffer(GL_ARRAY_BUFFER, id_tex_uvs);

	// 3. Set the Vertex Attribute Pointers

	 // Vertex Positions

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Vertex Normals

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// Vertex Texture Coordinates

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));

	//glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * 4, uvs, GL_STATIC_DRAW);

	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, index, GL_STATIC_DRAW);

	//glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 4, vertex, GL_STATIC_DRAW);


	// 4. Load data into Vertex Buffers

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	LOG("Mesh loaded with: %d vertices, %d indices", vertices.size(), indices.size());

	//glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);


	// 5. Unbind Buffers

	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	return true;
}

void UI_Bounds::RegenerateVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
}

void UI_Bounds::DeleteBuffers()
{

	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (EBO != 0) {
		glDeleteBuffers(1, &EBO);
		EBO = 0;
	}

	if (VAO != 0) {
		glDeleteBuffers(1, &VAO);
		VAO = 0;
	}


	//glDeleteBuffers(1, &id_tex_uvs);
	//id_tex_uvs = 0;
}


