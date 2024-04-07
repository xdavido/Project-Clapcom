#pragma once
#ifndef __UI_TEXT_H__
#define __UI_TEXT_H__
#pragma once

#include "Globals.h"
#include "C_UI.h"
#include "CMaterial.h"

#include "External/FreeType/include/ft2build.h"
#include "External/FreeType/include/freetype/freetype.h"
#include "External/FreeType/include/freetype/ftglyph.h"
#pragma comment(lib, "Source/External/FreeType/libx86/freetype.lib")

struct Character
{
	GLuint textureID;
	float2  size;
	float2  bearing;
	unsigned int advance;
};

struct Font
{
public:
	Font(std::string name, std::string fontPath = "Assets\\Fonts");
	~Font();

	bool InitFont(std::string name, std::string fontPath);

public:
	//
	std::string path;
	std::string name;

	//buffers
	float3 vertex[4];
	float2 uvs[4];

	uint* index;

	GLuint VBO; //vertex buffer object
	GLuint EBO; //element buffer object
	GLuint VAO;
	GLuint id_tex_uvs; //texture UVs

	GLuint textureID;

	//
	FT_Library ft;
	FT_Face face;

	std::map<char, std::unique_ptr<Character>> mCharacters;
};

class UI_Text : public C_UI
{
public:
	// x = 0, y = 0, w = 200, y = 50
	// t --> text / fs --> font size / ls --> line spacing
	UI_Text(GameObject* g, float x = 0, float y = 0, const char* t = "Hello World", float fs = 21, float ls = 0.5f, std::string fontName = "", std::string fontPath = "Assets\\Fonts", float w = 200, float h = 50, std::string shaderPath = "Assets/Shaders/UI Shader.glsl");
	~UI_Text();

	void OnInspector();

	void Draw(bool game) override;

	void SetText(const char* t);
	void SetFont(std::string name, std::string fontPath);
	void ChangeFontSize(float size);

public:
	std::string text;
	Font* font;

	float fontSize;
	float lineSpacing;

	UI_Bounds* boundsEditor;
	UI_Bounds* boundsGame;

	CMaterial* mat;

private:
	float space;
};
#endif // __UI_TEXT_H__