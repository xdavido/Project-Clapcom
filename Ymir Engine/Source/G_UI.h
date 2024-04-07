#ifndef __G_UI_H__
#define __G_UI_H__

#include "Application.h"

#include "Globals.h"
#include "GameObject.h"
#include "C_UI.h"

#include "CTransform.h"

#include "ModuleScene.h"

class UI_Canvas;

class UI_Image;
class UI_Text;
class UI_Button;
class UI_InputBox;
class UI_Checkbox;
class UI_Slider;

class G_UI : public GameObject
{
public:
	G_UI(UI_TYPE t, GameObject* pParent = External->scene->mRootNode, float x = 0, float y = 0);
	G_UI(GameObject* pParent = External->scene->mRootNode, float x = 0, float y = 0);
	~G_UI();

	update_status Update(float dt);
	C_UI* GetComponentUI(UI_TYPE type);

	bool AddUIComponent(UI_TYPE type, float x, float y, GameObject* parent = External->scene->mRootNode);

	UI_Image* AddImage(std::string imgPath = "Assets/InGameConeptPng.png", float x = 0, float y = 0, float w = 100, float h = 100, std::string shaderPath = "Assets/Shaders/UI Shader.glsl");
	UI_Text* AddText(const char* text, float x = 0, float y = 0, float fontSize = 21, float lineSpacing = 0.5f, std::string fontName = "", std::string fontPath = "Assets\\Fonts");
	UI_Button* AddButton(const char* text = "Button", float x = 0, float y = 0, std::string imgPath = "Assets/InGameConeptPng.png", float w = 300, float h = 50);
	UI_InputBox* AddInputBox(float x = 0, float y = 0, std::string imgPath = "Assets/InGameConeptPng.png", float w = 300, float h = 50);
	UI_Checkbox* AddCheckBox(bool checked = true, float x = 0, float y = 0, float w = 170, float h = 20);
	UI_Slider* AddSlider(bool floats = false, float min = 0, float max = 10, float value = 0, float x = 0, float y = 0, float w = 300, float h = 50, float hw = 70, float hh = 70);

public:
	UI_Canvas* canvas;

	//CTransform rectTransform;
};
#endif // __G_UI_H__