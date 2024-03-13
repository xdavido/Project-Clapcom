#include "ScriptEditor.h"
#include"ModuleMonoManager.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "Application.h"
#include "GameObject.h"

#include <filesystem>
#include "Log.h"

#include "External/mmgr/mmgr.h"

ScriptEditor::ScriptEditor()
{
	name = "Script Editor";
	txtName = "null";

	// Set Text Editor Language for editing the script, in this case, we are using C#.
	codeLanguage = TextEditor::LanguageDefinition::CPlusPlus();
	txtEditor.SetLanguageDefinition(codeLanguage);
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
			if (TimeManager::gameTimer.GetState() == TimerState::RUNNING)
			{
				LOG("[WARNING] You cannot save a script while running program");
				
			}
			else
			{

				std::string str = txtEditor.GetText();
				char* cstr = &str[0];

				SaveScriptTXT(str, txtName.c_str());
				External->moduleMono->ReCompileCS();
				//CMDCompileCS();
				//PhysfsEncapsule::SaveFile(("Assets/Scripts/" + txtName + ".cs").c_str(), cstr, str.length(), false);
			}
		}

		ImGui::Dummy(ImVec2(10, 10));

		ImGui::Text("Editing script: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), (!txtName.empty()) ? txtName.c_str() : "No script loaded");

		if (!txtName.empty())
			txtEditor.Render(txtName.c_str());
	}
	ImGui::End();

	//// Render the Script Editor
	//txtEditor.Render("Script Editor");

}


void ScriptEditor::CreateScriptTXT()
{
	txtEditor.SetText(baseSource);
	txtName = "New Script";
}

bool ScriptEditor::SaveScriptTXT(std::string scriptText, std::string fileName)
{
	bool ret = true;

	std::string fullPath = SCRIPTING_ASSETS_PATH + fileName;

	// Open the file for writing
	std::ofstream outputFile(fullPath);

	// Check if the file is opened successfully
	if (!outputFile.is_open()) {
		// Handle the error
		LOG("[ERROR] Unable to open file for writing - %s", fullPath.c_str());
		ret = false;
	}

	// Write the scriptText to the file
	outputFile << scriptText;

	// Close the file
	outputFile.close();

	// Check if the file is closed successfully
	if (!outputFile) {
		// Handle the error
		LOG("[ERROR] Failed to write content to file - %s", fullPath.c_str());
		ret = false;
	}

	//// Load Shader into memory to be able to be selected on the inspector
	//Shader* tmpShader = new Shader();
	//tmpShader->LoadShader(fullPath);
	//delete tmpShader;

	return ret;
}

void ScriptEditor::DeleteScriptTXT(std::string fileName)
{
	std::string fullPath = SCRIPTING_ASSETS_PATH + fileName + ".glsl";

	// Reset variables when deleting the current script
	txtEditor.SetText("");
	txtName = "";

	// Using std::filesystem::remove to delete the file
	if (std::filesystem::remove(fullPath))
	{
		// File deletion successful
		LOG("File deleted successfully: %s", fullPath.c_str());
	}
	else
	{
		// File deletion failed
		LOG("[ERROR] Unable to delete file: %s", fullPath.c_str());
	}

}

void ScriptEditor::LoadScriptTXT(std::string filePath)
{
	std::ifstream file;

	file.open(filePath);

	if (!file.is_open()) {

		LOG("[ERROR] Unable to open file.");

	}

	std::string fileContents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	file.close();

	// Retrieve the script file contents for the Scripting Editor
	txtName = std::filesystem::path(filePath).stem().string();
	txtEditor.SetText(fileContents);
}

void ScriptEditor::ShowNewScriptDialogue()
{
	static std::string newName = "New_Script";
	ImGui::Text("Name: ");
	ImGui::SameLine();
	if (ImGui::InputText(" ", &newName));

	ImGui::NewLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.0f, 0.0f, 1.0f)); // Set button color to red
	if (ImGui::Button("Close")) {

		External->editor->showNewScriptPopUp = false;

		ImGui::CloseCurrentPopup();

	}
	ImGui::PopStyleColor();

	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.6f, 0.0f, 1.0f)); // Set button color to green
	if (ImGui::Button("Create")) {

		newName = "Assets/Scripts/" + newName + ".cs";

		//Call fuction to add script to SLN
		External->moduleMono->CreateAssetsScript(newName.c_str());
		
		External->editor->showNewScriptPopUp = false;
		ImGui::CloseCurrentPopup();

	}
	ImGui::PopStyleColor();
}




//SUSU things
//void ScriptEditor::SetTextFromFile(const char* path)
//{
//	//txtEditor.Delete();
//
//	char* buffer = nullptr;
//	PhysfsEncapsule::LoadToBuffer(path, &buffer);
//
//	//std::string test = FileSystem::FileToText(path); //Can't use physFS because it's
//
//	if (buffer != nullptr)
//	{
//		txtName = path;
//		txtEditor.SetText(buffer);
//
//		RELEASE_ARRAY(buffer);
//	}
//	else
//	{
//		txtName = "";
//	}
//}