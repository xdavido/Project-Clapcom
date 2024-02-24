#pragma once

#include "PhysfsEncapsule.h"

#include "External/ImGuiTextEditor/include/TextEditor.h"

class Application;
class ScriptEditor
{

public:
	ScriptEditor();
	virtual ~ScriptEditor();


	// Main Functionality

	bool Init();
	void Draw();


	void SetTextFromFile(const char* path);

public:
	std::string txtName;
	std::string name;

private:
	TextEditor txtEditor;
};


