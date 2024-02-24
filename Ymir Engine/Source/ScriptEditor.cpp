#include "ScriptEditor.h"
#include"ModuleMonoManager.h"

#include <filesystem>

ScriptEditor::ScriptEditor()
{
	name = "Script Editor";
}

ScriptEditor::~ScriptEditor()
{

}

bool ScriptEditor::Init()
{
	return false;
}

void ScriptEditor::Draw()
{

	if (ImGui::Begin(name.c_str(), NULL /*| ImGuiWindowFlags_NoResize*//*, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse*/))
	{
		ImGui::Dummy(ImVec2(10, 10));
		if (ImGui::Button("Open Visual Studio Project"))
		{
			ShellExecute(0, 0, "Assembly-CSharp.sln", 0, 0, SW_SHOW);
			//EngineExternal->moduleMono->ReCompileCS();
		}
		ImGui::SameLine();
		if (ImGui::Button("Save and Reload Script"))
		{
			std::string str = txtEditor.GetText();
			char* cstr = &str[0];

			PhysfsEncapsule::SaveFile(txtName.c_str(), cstr, str.length(), false);
		}

		ImGui::Dummy(ImVec2(10, 10));

		ImGui::Text("Editing script: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), (!txtName.empty()) ? txtName.c_str() : "No script loaded");

		if (!txtName.empty())
			txtEditor.Render(txtName.c_str());
	}
	ImGui::End();

}

void ScriptEditor::SetTextFromFile(const char* path)
{
	//txtEditor.Delete();

	char* buffer = nullptr;
	PhysfsEncapsule::LoadToBuffer(path, &buffer);

	//std::string test = FileSystem::FileToText(path); //Can't use physFS because it's

	if (buffer != nullptr)
	{
		txtName = path;
		txtEditor.SetText(buffer);

		RELEASE_ARRAY(buffer);
	}
	else
	{
		txtName = "";
	}
}