#pragma once
#include "Module.h"

#include "External/mono/utils/mono-publib.h"
#include "External/MathGeoLib/include/Math/float3.h"

#include "External/mono/metadata/object-forward.h"
#include "External/mono/metadata/object.h"
#include "External/mono/metadata/blob.h"
#include "External/mono/metadata/threads.h"

typedef struct _MonoDomain MonoDomain;
typedef struct _MonoAssembly MonoAssembly;
typedef struct _MonoClassField MonoClassField;

class GameObject;
class CScript;

#define USER_SCRIPTS_NAMESPACE ""
#define YMIR_SCRIPTS_NAMESPACE "YmirEngine"

union FieldValue
{
	int iValue;
	float fValue;
	bool bValue;
	char strValue[50];
	GameObject* goValue;
};
struct SerializedField
{
	SerializedField();
	SerializedField(MonoClassField* _field, MonoObject* _object, CScript* parent);

	MonoClassField* field;
	MonoTypeEnum type = MonoTypeEnum::MONO_TYPE_END;
	FieldValue fiValue;
	CScript* parentSC;
	std::string displayName;
	uint goUID;
};

class ModuleMonoManager : public Module
{
public:
	ModuleMonoManager(Application* app, bool start_enabled = true);
	virtual ~ModuleMonoManager();

	bool Init() override;
	void OnGUI();
	bool CleanUp() override;

	static Quat UnboxQuat(MonoObject* _obj);
	void DebugAllFields(const char* className, std::vector<SerializedField>& _data, MonoObject* obj, CScript* script, const char* namespace_name);
	static float3 UnboxVector(MonoObject* _obj);

	static void LoadFieldData(SerializedField& _field, MonoObject* _object);
	static void DebugAllMethods(const char* nsName, const char* className, std::vector<std::string>& _data);
	

	void CreateAssetsScript(const char* localPath);
	void AddScriptToSLN(const char* scriptLocalPath);
	void RemoveScriptFromSLN(const char* scriptLocalPath);

	GameObject* GameObject_From_CSGO(MonoObject* goObj);

	MonoObject* QuatToCS(Quat& inVec) const;
	MonoObject* Float3ToCS(float3& inVec) const;
	MonoObject* GoToCSGO(GameObject* inGo) const;

	void ReCompileCS();

private:
	void InitMono();

public:
	MonoDomain* domain;
	MonoDomain* jitDomain;
	MonoAssembly* assembly;
	MonoImage* image;
	MonoThread* domainThread;

	std::vector<MonoClass*> userScripts;

private:

	std::string baseSourcePart1 = R"(using System;
		using System.Runtime.CompilerServices;
	using System.Runtime.InteropServices;

	using YmirEngine;

	public class )";
		
	std::string baseSourcePart2 = R"( : YmirComponent
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