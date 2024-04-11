#pragma once
#ifndef __UI_IMAGE_H__
#define __UI_IMAGE_H__
#pragma once

#include "Globals.h"
#include "C_UI.h"
#include "CMaterial.h"

class UI_Image : public C_UI
{
public:
	// x = 0, y = 0, w = 100, y = 100
	UI_Image(GameObject* g, float x = 0, float y = 0, float w = 100, float h = 100, std::string imgPath = "Assets/InGameConeptPng.png", std::string shaderPath = "Assets/Shaders/UI Shader.glsl");
	~UI_Image();

	void OnInspector();
	void Draw(bool game) override;
	update_status Update(float dt);

	void OnNormal();
	void OnFocused();
	void OnPressed();
	void OnSelected();
	void OnRelease();
	void OnDisabled();

	void SetImg(std::string imgPath, UI_STATE state);
	void SetStateImg(const char* label, UI_STATE s = UI_STATE::NONE);
	void SetNativeSize();
	void SetSpriteSize();

public:
	CMaterial* mat;
	ResourceTexture* selectedTexture;
	std::map<UI_STATE, ResourceTexture*> mapTextures;

	int ssCoordsY, ssCoordsX;
	int ssRows, ssColumns;
};
#endif // __UI_IMAGE_H__