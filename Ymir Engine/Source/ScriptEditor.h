#pragma once

#include "PhysfsEncapsule.h"

#include "External/ImGuiTextEditor/include/TextEditor.h"


#define SCRIPTING_ASSETS_PATH "Assets/Scripts/"
#define SCRIPTING_LIBRARY_PATH "Library/Scripts/"

class Application;
class ScriptEditor
{

public:
	ScriptEditor();
	virtual ~ScriptEditor();


	// Main Functionality

	bool Init();
	void Draw();


	void CreateScriptTXT();
	bool SaveScriptTXT(std::string shaderText, std::string fileName);
	void DeleteScriptTXT(std::string fileName);
	void LoadScriptTXT(std::string filePath);

	void ShowNewScriptDialogue();


	void SetTextFromFile(const char* path);

public:
	std::string txtName;
	std::string name;

private:
	TextEditor txtEditor;

	TextEditor::LanguageDefinition codeLanguage;

	std::string baseSource = R"(using System;
		using System.Runtime.CompilerServices;
	using System.Runtime.InteropServices;

	using YmirEngine;

	public class Core : YmirComponent
	{
		public GameObject reference = null;
		public GameObject turret = null;
		public GameObject shootPoint = null;

		public float rotationSpeed = 2.0f;
		public float movementSpeed = 35.0f;
		public float mouseSens = 5.0f;

		public bool testBool = false;
		public string testString = "Hello World";

		public Vector3 testOtherClass; //Should find a way to tell if the class is a gameobject or not


		public void Update(/*int x*/)
		{
			testString = "Still an Error";

			if (this.reference == null)
				return;

			testString = "Got It";

			if (Input.GetKey(YmirKeyCode.W) == KeyState.KEY_REPEAT)
				reference.localPosition += reference.GetForward() * movementSpeed * Time.deltaTime;
			if (Input.GetKey(YmirKeyCode.S) == KeyState.KEY_REPEAT)
				reference.localPosition += reference.GetForward() * -movementSpeed * Time.deltaTime;
			if (Input.GetKey(YmirKeyCode.A) == KeyState.KEY_REPEAT)
				reference.localRotation *= Quaternion.RotateAroundAxis(Vector3.up, rotationSpeed * Time.deltaTime);
			if (Input.GetKey(YmirKeyCode.D) == KeyState.KEY_REPEAT)
				reference.localRotation *= Quaternion.RotateAroundAxis(Vector3.up, -rotationSpeed * Time.deltaTime);

			//Destroy current GameObject - It works
			if (Input.GetKey(YmirKeyCode.X) == KeyState.KEY_REPEAT)
				InternalCalls.Destroy(reference);

			//Create a GameObject - Not working
			if (Input.GetKey(YmirKeyCode.C) == KeyState.KEY_REPEAT)
				InternalCalls.CreateGameObject("Cube", reference);

			//if (Input.GetMouseX() != 0 && turret != null)
			//    turret.localRotation = Quaternion.RotateAroundAxis(Vector3.up, -Input.GetMouseX() * mouseSens * Time.deltaTime) * turret.localRotation;

			//if (Input.GetMouseY() != 0 && turret != null)
			//    turret.localRotation = turret.localRotation * Quaternion.RotateAroundAxis(Vector3.right, -Input.GetMouseY() * Time.deltaTime);

			if (Input.GetMouseClick(MouseButton.LEFT) == KeyState.KEY_DOWN)
			{
				InternalCalls.CreateBullet(shootPoint.globalPosition, shootPoint.globalRotation, shootPoint.globalScale);
			}

			return;
		}
	}










	)";

};


