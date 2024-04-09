#include "UI_Text.h"
#include "UI_Transform.h"

#include "GameObject.h"

#include "ModuleEditor.h"

//#include "External/ImGui/imgui_custom.h"

#include "External/mmgr/mmgr.h"

UI_Text::UI_Text(GameObject* g, float x, float y, const char* t, float fs, float ls, std::string fontName, std::string fontPath, float w, float h, std::string shaderPath) : C_UI(UI_TYPE::TEXT, ComponentType::UI, g, "Text", x, y, w, h)
{
	text = t;

	if (fontName == "")
	{
		font = External->renderer3D->defaultFont;
	}
	else
	{
		SetFont(fontName, fontPath);
	}

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
	editorVertex1.position = float3::zero;
	editorVertex1.textureCoordinates = float2(0, 0);
	boundsEditor->vertices.push_back(editorVertex1);

	Vertex editorVertex2;
	editorVertex2.position = float3::zero;
	editorVertex2.textureCoordinates = float2(1, 0);
	boundsEditor->vertices.push_back(editorVertex2);

	Vertex editorVertex3;
	editorVertex3.position = float3::zero;
	editorVertex3.textureCoordinates = float2(0, 1);
	boundsEditor->vertices.push_back(editorVertex3);

	Vertex editorVertex4;
	editorVertex4.position = float3::zero;
	editorVertex4.textureCoordinates = float2(1, 1);
	boundsEditor->vertices.push_back(editorVertex4);

	Vertex gameVertex1;
	gameVertex1.position = float3::zero;
	gameVertex1.textureCoordinates = float2(0, 1);
	boundsGame->vertices.push_back(gameVertex1);

	Vertex gameVertex2;
	gameVertex2.position = float3::zero;
	gameVertex2.textureCoordinates = float2(1, 1);
	boundsGame->vertices.push_back(gameVertex2);

	Vertex gameVertex3;
	gameVertex3.position = float3::zero;
	gameVertex3.textureCoordinates = float2(0, 0);
	boundsGame->vertices.push_back(gameVertex3);

	Vertex gameVertex4;
	gameVertex4.position = float3::zero;
	gameVertex4.textureCoordinates = float2(1, 0);
	boundsGame->vertices.push_back(gameVertex4);

	boundsEditor->InitBuffers();
	boundsGame->InitBuffers();

	fontSize = fs;
	lineSpacing = ls;
	space = 0;

	mat = new CMaterial(g);
	mat->shaderPath = shaderPath;
	mat->shader.LoadShader(mat->shaderPath);

	tabNav_ = false;
}

UI_Text::~UI_Text()
{
	font = nullptr;

	boundsEditor->DeleteBuffers();
	boundsGame->DeleteBuffers();

	//RELEASE_ARRAY(boundsEditor->index);
	//RELEASE_ARRAY(boundsGame->index);
	RELEASE(boundsEditor);
	RELEASE(boundsGame);
}

void UI_Text::OnInspector()
{
	bool exists = true;
	ImGui::Checkbox(("##UI Text Checkbox" + std::to_string(mOwner->UID)).c_str(), &active);		ImGui::SameLine();

	if (ImGui::CollapsingHeader(("Text##UI Collapsing Header" + std::to_string(mOwner->UID)).c_str(), &exists, ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (!active) { ImGui::BeginDisabled(); }

		ImGui::TextColored(WARNING_COLOR, "[!] Move with UI Transform");

		ImGui::Checkbox("Draggeable", &isDraggeable);

		ImGui::InputTextMultiline(("Text##" + std::to_string(mOwner->UID)).c_str(), &text, ImVec2(0, 0), ImGuiInputTextFlags_AllowTabInput);
		//ImGui::InputText(name.c_str(), &text, ImGuiInputTextFlags_EnterReturnsTrue);
		ImGui::Dummy(ImVec2(0, 10));

		if (ImGui::BeginCombo("Font", font->name.c_str()))
		{
			for (std::vector<Font*>::const_iterator it = External->renderer3D->mFonts.begin(); it != External->renderer3D->mFonts.end(); ++it)
			{
				bool isSelected = (font->name == (*it)->name);
				if (ImGui::Selectable((*it)->name.c_str()))
				{
					font = (*it);
				}
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("font"))
			{
				std::string path = (const char*)payload->Data;
				std::string p, n, e;

				std::size_t it = path.find(".ttf");

				if (it == std::string::npos)
				{
					it = path.find(".otf");
				}
				path.erase(it + 4);

				PhysfsEncapsule::SplitFilePath(path.c_str(), &p, &n, &e);

				p.erase(p.find_last_of("/"));
				SetFont(n + "." + e, p);
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::Text("Font Size");
		ImGui::DragFloat("##FontSize", (fontSize < 0) ? &(fontSize = 0) : &fontSize, 0.1f, 0, 0, "%.1f");

		ImGui::Text("Line Spacing");
		ImGui::DragFloat("##LineSpacing", &lineSpacing, 0.1f);

		//ImGui::ColorEdit4("Color", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

		if (!active) { ImGui::EndDisabled(); }
	}

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

	ImGui::Checkbox(("##" + mOwner->name + std::to_string(ctype)).c_str(), &active);
	ImGui::SameLine();

	if (ImGui::CollapsingHeader("Material", &exists, flags))
	{
		ImGui::Indent();

		ImGui::Spacing();

		if (!active) { ImGui::BeginDisabled(); }

		ImGui::Spacing();

		// ------------------------------------ SHADER ------------------------------------

		ImGui::SeparatorText("SHADER");

		ImGui::Spacing();

		ImGui::Text("Shader: ");
		ImGui::SameLine();

		if (mOwner->selected) {

			// Find the index of the current shader path in listShaderPaths
			auto it = std::find(listShaderPaths.begin(), listShaderPaths.end(), mat->shaderPath);

			if (it != listShaderPaths.end()) {

				mat->selectedShader = static_cast<int>(std::distance(listShaderPaths.begin(), it));

			}

		}

		// Choose between the list of shaders
		if (ImGui::Combo("##ChooseShader", &mat->selectedShader, listShaderNames.data(), listShaderNames.size())) {

			mat->shaderDirtyFlag = true;

		}

		if (mat->shaderDirtyFlag) {

			// When shader changes, update the shader path and recompile
			mat->shaderPath = listShaderPaths[mat->selectedShader];
			mat->shader.LoadShader(mat->shaderPath);

			// Reset the dirty flag after handling the change
			mat->shaderDirtyFlag = false;

		}

		ImGui::Spacing();

		// Shader Uniforms Management

		if (mOwner->selected) {

			if (mat->shader.uniforms.size() == 0) {

				ImGui::Text("No editable uniforms.");

			}
			else {

				ImGui::Text("Uniforms:");

			}

			ImGui::Spacing();

			ImGui::Indent();

			// In case the shader has editable uniforms:
			for (auto kt = mat->shader.uniforms.begin(); kt != mat->shader.uniforms.end(); ++kt) {

				std::string label = "##" + kt->name;

				ImGui::Text("%s", kt->name.c_str());
				ImGui::SameLine();

				// Change display according to uniform type
				switch (kt->type)
				{
				case UniformType::boolean:

					ImGui::Checkbox(label.c_str(), (bool*)kt->value.get());

					mat->shader.SetUniformValue(kt->name, (bool*)kt->value.get());

					break;

				case UniformType::i1:

					ImGui::DragInt(label.c_str(), (int*)kt->value.get(), 0.01f);

					mat->shader.SetUniformValue(kt->name, (int*)kt->value.get());

					break;

				case UniformType::i2:

					ImGui::DragInt2(label.c_str(), (int*)kt->value.get(), 0.01f);

					mat->shader.SetUniformValue(kt->name, (int*)kt->value.get());

					break;

				case UniformType::i3:

					ImGui::DragInt3(label.c_str(), (int*)kt->value.get(), 0.01f);

					mat->shader.SetUniformValue(kt->name, (int*)kt->value.get());

					break;

				case UniformType::i4:

					ImGui::DragInt4(label.c_str(), (int*)kt->value.get(), 0.01f);

					mat->shader.SetUniformValue(kt->name, (int*)kt->value.get());

					break;

				case UniformType::f1:

					ImGui::DragFloat(label.c_str(), (float*)kt->value.get(), 0.01f);

					mat->shader.SetUniformValue(kt->name, (float*)kt->value.get());

					break;

				case UniformType::f2:

					ImGui::DragFloat2(label.c_str(), (float*)kt->value.get(), 0.01f);

					mat->shader.SetUniformValue(kt->name, (float*)kt->value.get());

					break;

				case UniformType::f3:

					ImGui::DragFloat3(label.c_str(), (float*)kt->value.get(), 0.01f);

					mat->shader.SetUniformValue(kt->name, (float*)kt->value.get());

					break;

				case UniformType::f4:

					ImGui::DragFloat4(label.c_str(), (float*)kt->value.get(), 0.01f);

					mat->shader.SetUniformValue(kt->name, (float*)kt->value.get());

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

		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(mat->diffuse_ID)), textureMapSize);
		mat->DdsDragDropTarget();
		ImGui::SameLine();
		ImGui::Text("Diffuse");
		ImGui::SameLine();
		ImGui::Text("(%s)", mat->diffuse_path.c_str());

		ImGui::Spacing();

		ImGui::ColorButton("Specular", ImVec4(0, 0, 0, 0), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, textureMapSize);
		mat->DdsDragDropTarget();
		ImGui::SameLine();
		ImGui::Text("Specular");

		ImGui::Spacing();

		ImGui::ColorButton("Normal", ImVec4(0, 0, 0, 0), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, textureMapSize);
		mat->DdsDragDropTarget();
		ImGui::SameLine();
		ImGui::Text("Normal");

		ImGui::Spacing();

		ImGui::ColorButton("Height", ImVec4(0, 0, 0, 0), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, textureMapSize);
		mat->DdsDragDropTarget();
		ImGui::SameLine();
		ImGui::Text("Height");

		ImGui::Spacing();

		ImGui::ColorButton("Ambient", ImVec4(0, 0, 0, 0), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, textureMapSize);
		mat->DdsDragDropTarget();
		ImGui::SameLine();
		ImGui::Text("Ambient");

		ImGui::Spacing();

		ImGui::ColorButton("Emissive", ImVec4(0, 0, 0, 0), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, textureMapSize);
		mat->DdsDragDropTarget();
		ImGui::SameLine();
		ImGui::Text("Emissive");

		ImGui::Spacing();

		// Utility buttons

		if (ImGui::Button("Apply Checker Texture")) {

			External->renderer3D->ApplyCheckerTexture();

		}

		ImGui::Spacing();

		if (ImGui::Button("Clear Actual Texture")) {

			External->renderer3D->ClearActualTexture();

		}

		if (!active) { ImGui::EndDisabled(); }

		ImGui::Spacing();

		ImGui::Unindent();
	}

	if (!exists) { mOwner->RemoveComponent(this); }
}

void UI_Text::Draw(bool game)
{
	UI_Bounds* boundsDrawn = nullptr;
	space = 0;

	float fs = fontSize * 1.5;
	uint newLines = 0;

	for (size_t i = 0; i < text.length(); ++i)
	{
		float3 position = mOwner->mTransform->translation;

		auto itr = font->mCharacters.find(text[i]);

		if (itr != font->mCharacters.end())
		{
			float sizeX = itr->second->size.x * (fs / 98);
			float sizeY = itr->second->size.y * (fs / 98);

			float nlOffsetE = (/*position.y +*/ sizeY + mOwner->mTransform->GetGlobalPosition().y + (fs * lineSpacing)) * newLines;
			float nlOffsetG = (/*posY +*/ (scaleBounds.y * mOwner->mTransform->scale.y) + mOwner->mTransform->GetGlobalPosition().y + (fs * lineSpacing)) / 2 * newLines;

			if (i != 0)
			{
				auto itr2 = font->mCharacters.find(text[i - 1]);

				if (itr2 == font->mCharacters.end())
				{
					return;
				}

				if (itr2->first != '\n')
				{
					space = space + itr2->second->size.x * (fs / 98) + (fs / 5);
				}
			}

			if (itr->first == ' ')
			{
				space += (fs / 2);
			}

			if (itr->first == '\n')
			{
				++newLines;
				space = 0;
			}
			else
			{
				boundsEditor->vertices[0].position = float3(position.x + space + mOwner->mTransform->GetGlobalPosition().x, position.y + sizeY + mOwner->mTransform->GetGlobalPosition().y + nlOffsetE, 0);
				boundsEditor->vertices[1].position = float3(position.x + space + (sizeX * scaleBounds.x * mOwner->mTransform->scale.x) + mOwner->mTransform->GetGlobalPosition().x, position.y + sizeY + mOwner->mTransform->GetGlobalPosition().y + nlOffsetE, 0);
				boundsEditor->vertices[2].position = float3(position.x + space + mOwner->mTransform->GetGlobalPosition().x, position.y + mOwner->mTransform->GetGlobalPosition().y + nlOffsetE, 0);
				boundsEditor->vertices[3].position = float3(position.x + space + (sizeX * scaleBounds.x * mOwner->mTransform->scale.x) + mOwner->mTransform->GetGlobalPosition().x, position.y + mOwner->mTransform->GetGlobalPosition().y + nlOffsetE, 0);

				// Bot left - Bot right - Top left - Top right
				boundsGame->vertices[0].position = float3(posX + space + mOwner->mTransform->GetGlobalPosition().x, posY + (scaleBounds.y * mOwner->mTransform->scale.y) + mOwner->mTransform->GetGlobalPosition().y + nlOffsetG, 0);
				boundsGame->vertices[1].position = float3(posX + space + (sizeX * scaleBounds.x * mOwner->mTransform->scale.x) + mOwner->mTransform->GetGlobalPosition().x, posY + (scaleBounds.y * mOwner->mTransform->scale.y) + mOwner->mTransform->GetGlobalPosition().y + nlOffsetG, 0);
				boundsGame->vertices[2].position = float3(posX + space + mOwner->mTransform->GetGlobalPosition().x, posY + (scaleBounds.y * mOwner->mTransform->scale.y) - sizeY + mOwner->mTransform->GetGlobalPosition().y + nlOffsetG, 0);
				boundsGame->vertices[3].position = float3(posX + space + (sizeX * scaleBounds.x * mOwner->mTransform->scale.x) + mOwner->mTransform->GetGlobalPosition().x, posY + (scaleBounds.y * mOwner->mTransform->scale.y) - sizeY + mOwner->mTransform->GetGlobalPosition().y + nlOffsetG, 0);

				boundsEditor->RegenerateVBO();
				boundsGame->RegenerateVBO();

				if (game)
				{
					boundsDrawn = boundsGame;
				}

				else
				{
					boundsDrawn = boundsEditor;

					//glPushMatrix();
					//glMultMatrixf(mOwner->mTransform->mGlobalMatrix.Transposed().ptr());
				}

				// Render

				glBindTexture(GL_TEXTURE_2D, itr->second->textureID);

				mat->shader.UseShader(true);
				mat->shader.SetShaderUniforms(&transformUI->mMatrixUI, mOwner->selected);

				glBindVertexArray(boundsDrawn->VAO);

				glDrawElements(GL_TRIANGLES, boundsDrawn->indices.size(), GL_UNSIGNED_INT, 0);

				glBindVertexArray(0);

				mat->shader.UseShader(false);

				glBindTexture(GL_TEXTURE_2D, 0);

				if (!game)
				{
					//glPopMatrix();
				}
			}
		}
	}

	boundsDrawn = nullptr;
}

void UI_Text::SetText(const char* t)
{
	text = t;
}

void UI_Text::SetFont(std::string name, std::string fontPath)
{
	bool isImported = false;
	for (auto it = External->renderer3D->mFonts.begin(); it != External->renderer3D->mFonts.end(); ++it)
	{
		if ((*it)->name == name)
		{
			isImported = true;
			font = (*it);
		}
	}

	if (!isImported)
	{
		font = new Font(name, fontPath);
	}
}

void UI_Text::ChangeFontSize(float size)
{
	fontSize = size;
}

Font::Font(std::string name, std::string fontPath)
{
	InitFont(name, fontPath);

	// 128 --> number of characters in a font
	FT_Set_Pixel_Sizes(face, 0, 128);

	// disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// load first 128 characters of ASCII set
	for (unsigned char c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			LOG("[ERROR] Failed to load glyph");
			continue;
		}

		// generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// Allocate a new buffer for RGBA data
		GLubyte* rgbaBuffer = new GLubyte[4 * face->glyph->bitmap.width * face->glyph->bitmap.rows];

		// Copy the glyph data into the RGBA buffer
		for (int y = 0; y < face->glyph->bitmap.rows; ++y)
		{
			for (int x = 0; x < face->glyph->bitmap.width; ++x)
			{
				GLubyte value = face->glyph->bitmap.buffer[y * face->glyph->bitmap.width + x];
				rgbaBuffer[4 * (y * face->glyph->bitmap.width + x)] = value;  // Red
				rgbaBuffer[4 * (y * face->glyph->bitmap.width + x) + 1] = value;  // Green
				rgbaBuffer[4 * (y * face->glyph->bitmap.width + x) + 2] = value;  // Blue
				rgbaBuffer[4 * (y * face->glyph->bitmap.width + x) + 3] = value;  // Alpha
			}
		}

		// Use the RGBA buffer for texture data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, rgbaBuffer);

		// Clean up the temporary buffer
		delete[] rgbaBuffer;

		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// TODO: MEMORY LEAK -> Smart pointers don't work here
		Character* chara(new Character
			{
				texture,
				float2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				float2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x),
			});

		mCharacters.insert(std::pair<GLchar, Character*>(c, chara));
	}

	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	LOG("Font loaded: %s", name.c_str());
	External->renderer3D->mFonts.push_back(this);
}

Font::~Font()
{
	//for (unsigned char c = 0; c < 128; ++c)
	//{
	//	RELEASE(mCharacters[c]);
	//}

	mCharacters.clear();
}

bool Font::InitFont(std::string n, std::string fontPath)
{
	if (FT_Init_FreeType(&ft))
	{
		LOG("[ERROR] Could not init FreeType Library");
		return false;
	}

	if (FT_New_Face(ft, (fontPath + "/" + n).c_str(), 0, &face))
	{
		LOG("[ERROR] Failed to load font");
		return false;
	}

	path = fontPath;
	name = n;

	return true;
}
