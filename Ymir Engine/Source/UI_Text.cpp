#include "UI_Text.h"
#include "GameObject.h"

#include "ModuleEditor.h"

//#include "External/ImGui/imgui_custom.h"

UI_Text::UI_Text(GameObject* g, int x, int y, int w, int h) : C_UI(UI_TYPE::TEXT, ComponentType::UI, g, "Text", x, y, w, h)
{
	text = "Hello World";
	font = External->renderer3D->defaultFont;

	boundsEditor = new UIBounds;
	boundsGame = new UIBounds;

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
	gameVertex1.textureCoordinates = float2(0, 0);
	boundsGame->vertices.push_back(gameVertex1);

	Vertex gameVertex2;
	gameVertex2.position = float3::zero;
	gameVertex2.textureCoordinates = float2(1, 0);
	boundsGame->vertices.push_back(gameVertex2);

	Vertex gameVertex3;
	gameVertex3.position = float3::zero;
	gameVertex3.textureCoordinates = float2(0, 1);
	boundsGame->vertices.push_back(gameVertex3);

	Vertex gameVertex4;
	gameVertex4.position = float3::zero;
	gameVertex4.textureCoordinates = float2(1, 1);
	boundsGame->vertices.push_back(gameVertex4);

	boundsEditor->InitBuffers();
	boundsGame->InitBuffers();

	fontSize = 21;
	space = 0;
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

		ImGui::Checkbox("Draggeable", &isDraggable);

		ImGui::InputTextMultiline(("Text##"+ std::to_string(mOwner->UID)).c_str(), &text, ImVec2(0, 0), ImGuiInputTextFlags_AllowTabInput);
		//ImGui::InputText(name.c_str(), &text, ImGuiInputTextFlags_EnterReturnsTrue);
		ImGui::Dummy(ImVec2(0, 10));

		if (ImGui::Button("change font1"))
		{
			//RELEASE(font);
			font = new Font("Arial.ttf",  "Assets\\Fonts");
		}
		//if (ImGui::Button("change eglantineVar2"))
		//{
		//	//RELEASE(font);
		//	font = new Font("eglantineVar2.ttf",  "Assets\\Fonts");
		//}
		if (ImGui::Button("change comic"))
		{
			//RELEASE(font);
			font = new Font("comic.ttf",  "Assets\\Fonts");
		}
		if (ImGui::Button("change times"))
		{
			//RELEASE(font);
			font = new Font("times.ttf",  "Assets\\Fonts");
		}
		if (ImGui::Button("change consola"))
		{
			//RELEASE(font);
			font = new Font("default_consola.ttf",  "Assets\\Fonts");
		}
		if (ImGui::Button("change Drawing with markers"))
		{
			//RELEASE(font);
			font = new Font("Drawing with markers.ttf",  "Assets\\Fonts");
		}
		//if (ImGui::Button("change Cat Paw"))
		//{
		//	//RELEASE(font);
		//	font = new Font("Cat Paw.otf",  "Assets\\Fonts");
		//}
		if (ImGui::Button("change times"))
		{
			//RELEASE(font);
			font = new Font("times.ttf", "Assets\\Fonts");
		}

		ImGui::Text("Font Size");
		if (ImGui::DragFloat("##FontSize", (fontSize < 0) ? &(fontSize = 0) : &fontSize, 0.1f, 0, 0, "%.1f"))
		{
		}
		ImGui::ColorEdit4("Color", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

		if (!active) { ImGui::EndDisabled(); }
	}
	ImGui::SameLine();

	if (!exists) { mOwner->RemoveComponent(this); }
}

void UI_Text::Draw(bool game)
{
	UIBounds* boundsDrawn = nullptr;
	space = 0;

	for (size_t i = 0; i < text.length(); i++)
	{
		float3 position = mOwner->mTransform->translation;

		auto itr = font->mCharacters.find(text[i]);

		if (itr != font->mCharacters.end())
		{
			float sizeX = itr->second->size.x * (fontSize / 98);

			if (i != 0)
			{
				auto itr2 = font->mCharacters.find(text[i - 1]);
				space = space + itr2->second->size.x * (fontSize / 98) + (fontSize / 5);
			}

			if (itr->first == ' ')
			{
				space += fontSize;
			}

			boundsEditor->vertices[0].position = float3(position.x + space, position.y + (fontSize * scaleBounds.y), 0);
			boundsEditor->vertices[1].position = float3(position.x + space + (sizeX * scaleBounds.x), position.y + (fontSize * scaleBounds.y), 0);
			boundsEditor->vertices[2].position = float3(position.x + space, position.y, 0);
			boundsEditor->vertices[3].position = float3(position.x + space + (sizeX * scaleBounds.x), position.y, 0);

			boundsGame->vertices[0].position = float3(posX + space, posY + (fontSize * scaleBounds.y), 0);
			boundsGame->vertices[1].position = float3(posX + space + (sizeX * scaleBounds.x), posY + (fontSize * scaleBounds.y), 0);
			boundsGame->vertices[2].position = float3(posX + space, posY, 0);
			boundsGame->vertices[3].position = float3(posX + space + (sizeX * scaleBounds.x), posY, 0);

			boundsEditor->RegenerateVBO();
			boundsGame->RegenerateVBO();

			if (game)
			{
				boundsDrawn = boundsGame;

				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(0.0, External->editor->gameViewSize.x, 0.0, External->editor->gameViewSize.y, 1.0, -1.0);

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
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
		}
	}

	boundsDrawn = nullptr;
}

void UI_Text::ChangeFontSize()
{
	UIBounds* boundsDrawn = nullptr;
	space = 0;

	for (size_t i = 0; i < text.length(); i++)
	{
		float3 position = mOwner->mTransform->translation;

		auto itr = font->mCharacters.find(text[i]);

		if (itr != font->mCharacters.end())
		{
			if (i != 0)
			{
				auto itr2 = font->mCharacters.find(text[i - 1]);
				space += itr2->second->size.x;
			}

			if (itr->first == ' ')
			{
				space += fontSize;
			}

			boundsEditor->vertices[0].position = float3(position.x + space, position.y + (fontSize * scaleBounds.y), 0);
			boundsEditor->vertices[1].position = float3(position.x + space + (itr->second->size.x * scaleBounds.x), position.y + (fontSize * scaleBounds.y), 0);
			boundsEditor->vertices[2].position = float3(position.x + space, position.y, 0);
			boundsEditor->vertices[3].position = float3(position.x + space + (itr->second->size.x * scaleBounds.x), position.y, 0);

			boundsGame->vertices[0].position = float3(posX + space, posY + (fontSize * scaleBounds.y), 0);
			boundsGame->vertices[1].position = float3(posX + space + (itr->second->size.x * scaleBounds.x), posY + (fontSize * scaleBounds.y), 0);
			boundsGame->vertices[2].position = float3(posX + space, posY, 0);
			boundsGame->vertices[3].position = float3(posX + space + (itr->second->size.x * scaleBounds.x), posY, 0);

			boundsEditor->RegenerateVBO();
			boundsGame->RegenerateVBO();
		}
	}

	boundsDrawn = nullptr;
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

		// now store character for later use
		Character* character = new Character
		{
			texture,
			float2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			float2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<unsigned int>(face->glyph->advance.x),
		};

		mCharacters.insert(std::pair<GLchar, Character*>(c, character));
	}

	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

Font::~Font()
{
	for (unsigned char c = 0; c < 128; ++c)
	{
		RELEASE(mCharacters[c]);
	}

	mCharacters.clear();
}

bool Font::InitFont(std::string name, std::string fontPath)
{
	if (FT_Init_FreeType(&ft))
	{
		LOG("[ERROR] Could not init FreeType Library");
		return false;
	}

	if (FT_New_Face(ft, (fontPath + "/" + name).c_str(), 0, &face))
	{
		LOG("[ERROR] Failed to load font");
		return false;
	}
	return true;
}

GLuint Font::GetCharacterTexID(GLchar character)
{
	for (std::map<GLchar, Character*>::const_iterator it = mCharacters.begin(); it != mCharacters.end(); it++)
	{
		if ((*it).first == character)
		{
			GLuint id = (*it).second->textureID;
			return id;
		}
	}

	return 0;
}
