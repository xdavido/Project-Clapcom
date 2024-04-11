#pragma once

#include "Globals.h"
#include "Application.h"

#include "GameObject.h"
#include "ResourceMesh.h"

#include "CMesh.h"
#include "CScript.h"
#include "CTransform.h"

#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "ModuleResourceManager.h" 
#include "ModuleMonoManager.h"
#include "Resources.h"
#include "PhysfsEncapsule.h"

#include "G_UI.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "UI_Button.h"
#include "UI_InputBox.h"
#include "UI_CheckBox.h"
#include "UI_Slider.h"

#include "MathGeoLib/include/Math/float3.h"

template<typename T>
T CS_CompToComp(MonoObject* obj)
{
	uintptr_t ptr = 0;
	MonoClass* goClass = mono_object_get_class(obj);
	const char* name = mono_class_get_name(goClass);

	mono_field_get_value(obj, mono_class_get_field_from_name(goClass, "pointer"), &ptr);

	return reinterpret_cast<T>(ptr);
}
//------//
MonoObject* Ymir_Box_Vector(MonoObject* obj, const char* type, bool global)	//Retorna la nueva posici�n del objeto
{
	//TODO: Quitar esto mas adelante, cuando est� arreglado el Transform


	if (External == nullptr)
		return nullptr;

	const char* name = mono_class_get_name(mono_object_get_class(obj));

	float3 value;
	CTransform* workTrans = CS_CompToComp<CTransform*>(obj);

	if (strcmp(type, "POSITION") == 0)
	{
		(global == true) ? value = workTrans->mGlobalMatrix.TranslatePart() : value = workTrans->translation;
	}
	else
	{
		(global == true) ? value = workTrans->mGlobalMatrix.GetScale() : value = workTrans->scale;
	}

	return External->moduleMono->Float3ToCS(value);
}
MonoObject* Ymir_Box_Quat(MonoObject* obj, bool global)	//Retorna la nueva rotaci�n del objeto
{
	//TODO: Quitar esto mas adelante, cuando est� arreglado el Transform

	if (External == nullptr)
		return nullptr;

	const char* name = mono_class_get_name(mono_object_get_class(obj));

	Quat value;
	GameObject* workGO = External->moduleMono->GameObject_From_CSGO(obj);

	CTransform* workTrans = CS_CompToComp<CTransform*>(obj);
	if (global = true) {
		float3 pos, scale;
		Quat globalRot;
		workTrans->mGlobalMatrix.Decompose(pos, globalRot, scale);
		value = globalRot;
	}
	else
	{
		value = workTrans->rotation;
	}


	return External->moduleMono->QuatToCS(value);
}

#pragma region Internals
//-------------------------------------------- Internals -----------------------------------------------//
void CSLog(MonoString* x)
{
	if (x == NULL)
		return;

	char* msg = mono_string_to_utf8(x);
	LOG(msg);
	mono_free(msg);
}

int GetKey(MonoObject* x)
{
	if (External != nullptr)
		return External->input->GetKey(*(int*)mono_object_unbox(x));

	return 0;
}

//Hardcoded para que solo sea KEY_DOWN y A
bool IsGamepadButtonAPressedCS()
{
	return External->input->IsGamepadButtonPressed(SDL_CONTROLLER_BUTTON_A, KEY_DOWN);
}

//Hardcoded para que solo sea KEY_DOWN y B
bool IsGamepadButtonBPressedCS()
{
	return External->input->IsGamepadButtonPressed(SDL_CONTROLLER_BUTTON_B, KEY_DOWN);
}

int GetMouseClick(MonoObject* x)
{
	if (External != nullptr)
		return External->input->GetMouseButton(*(int*)mono_object_unbox(x));

	return 0;
}
int MouseX()
{
	if (External != nullptr)
		return External->input->GetMouseXMotion();

	return 0;
}
int MouseY()
{
	if (External != nullptr)
		return External->input->GetMouseYMotion();

	return 0;
}

void CSCreateGameObject(MonoObject* name, MonoObject* position)
{
	if (External == nullptr)
		return;

	char* p = mono_string_to_utf8(mono_object_to_string(name, NULL));
	GameObject* go = External->scene->PostUpdateCreateGameObject(p, External->scene->mRootNode);
	mono_free(p);

	float3 posVector = ModuleMonoManager::UnboxVector(position);

	go->mTransform->SetPosition(posVector);
	//go->mTransform->updateTransform = true;	//TODO: No tenemos la variable esta "updateTransform"


}
MonoObject* CS_GetComponent(MonoObject* ref, MonoString* type, int inputType)
{
	ComponentType sType = static_cast<ComponentType>(inputType);

	char* name = mono_string_to_utf8(type);
	Component* component = External->moduleMono->GameObject_From_CSGO(ref)->GetComponent(sType, name);
	mono_free(name);

	//assert(component != nullptr, "Trying to get a null component");
	if (component == nullptr)
		return nullptr;

	if (sType == ComponentType::SCRIPT)
		return mono_gchandle_get_target(dynamic_cast<CScript*>(component)->noGCobject);

	MonoClass* cmpClass = mono_object_get_class(ref);
	MonoObject* ret = mono_object_new(External->moduleMono->domain, cmpClass);

	//Get type from unity

	//Get type
	MonoClassField* field = mono_class_get_field_from_name(cmpClass, "pointer");

	uintptr_t goPtr = reinterpret_cast<uintptr_t>(component);
	mono_field_set_value(ret, field, &goPtr);

	return ret;
}

GameObject* CS_Comp_To_GameObject(MonoObject* component)
{
	uintptr_t ptr = 0;
	MonoClass* goClass = mono_object_get_class(component);

	mono_field_get_value(component, mono_class_get_field_from_name(goClass, "pointer"), &ptr);

	return reinterpret_cast<Component*>(ptr)->mOwner;
}

MonoObject* CS_Component_Get_GO(MonoObject* thisRef)
{
	return External->moduleMono->GoToCSGO(CS_Comp_To_GameObject(thisRef));
}

MonoString* Get_GO_Name(MonoObject* go)
{
	if (External == nullptr)
		return nullptr;

	return mono_string_new(
		External->moduleMono->domain,
		External->moduleMono->GameObject_From_CSGO(go)->name.c_str());
}

MonoObject* FindObjectWithUID(int id)
{
	return External->moduleMono->GoToCSGO(External->scene->mRootNode->FindChild(id));
}

MonoObject* FindObjectWithName(MonoString* name) {

	std::vector<GameObject*> gameObjectVec;
	External->scene->mRootNode->CollectChilds(gameObjectVec);

	if (name == NULL) {
		assert("The name you passed is null. >:/");
		return nullptr;
	}

	char* _name = mono_string_to_utf8(name);

	for (int i = 0; i < gameObjectVec.size(); i++) {

		if (strcmp(gameObjectVec[i]->name.c_str(), _name) == 0) {

			return External->moduleMono->GoToCSGO(gameObjectVec[i]);

		}

	}
	mono_free(_name);

	assert("The object you searched for doesn't exist. :/");
	return nullptr;

}

void SetImpulse(MonoObject* obj, MonoObject* vel) {

	if (External == nullptr)
		return;

	float3 omgItWorks = External->moduleMono->UnboxVector(vel);
	GameObject* cpp_gameObject = External->moduleMono->GameObject_From_CSGO(obj);
	CCollider* rigidbody = dynamic_cast<CCollider*>(cpp_gameObject->GetComponent(ComponentType::PHYSICS));

	if (rigidbody)
	{
		rigidbody->physBody->body->applyCentralImpulse({ omgItWorks.x, omgItWorks.y,omgItWorks.z });

	}

}

void SetVelocity(MonoObject* obj, MonoObject* vel) {

	if (External == nullptr)
		return;

	float3 omgItWorks = External->moduleMono->UnboxVector(vel);
	GameObject* cpp_gameObject = External->moduleMono->GameObject_From_CSGO(obj);
	CCollider* rigidbody = dynamic_cast<CCollider*>(cpp_gameObject->GetComponent(ComponentType::PHYSICS));

	if (rigidbody)
	{
		rigidbody->physBody->body->activate(true);
		rigidbody->physBody->body->setLinearVelocity({ omgItWorks.x, omgItWorks.y,omgItWorks.z });

	}

}

void SetRotation(MonoObject* obj, MonoObject* vel) {

	if (External == nullptr)
		return;

	Quat omgItWorks = External->moduleMono->UnboxQuat(vel);
	GameObject* cpp_gameObject = External->moduleMono->GameObject_From_CSGO(obj);
	CCollider* rigidbody = dynamic_cast<CCollider*>(cpp_gameObject->GetComponent(ComponentType::PHYSICS));

	if (rigidbody)
	{
		rigidbody->physBody->SetRotation(omgItWorks);

	}

}

void SetPosition(MonoObject* obj, MonoObject* pos) {
	if (External == nullptr)
		return;

	float3 omgItWorks = External->moduleMono->UnboxVector(pos);
	GameObject* cpp_gameObject = External->moduleMono->GameObject_From_CSGO(obj);
	CCollider* rigidbody = dynamic_cast<CCollider*>(cpp_gameObject->GetComponent(ComponentType::PHYSICS));

	if (rigidbody)
	{
		rigidbody->physBody->SetPosition(omgItWorks);

	}

}

MonoObject* SendPosition(MonoObject* obj) //Allows to send float3 as "objects" in C#, should find a way to move Vector3 as class
{
	//return mono_value_box(External->moduleMono->domain, vecClass, External->moduleMono->Float3ToCS(C_Script::runningScript->GetGO()->transform->position)); //Use this method to send "object" types
	return Ymir_Box_Vector(obj, "POSITION", false); //Use this method to send class types
}

void RecievePosition(MonoObject* obj, MonoObject* secObj) //Allows to send float3 as "objects" in C#, should find a way to move Vector3 as class
{
	if (External == nullptr)
		return;

	float3 omgItWorks = External->moduleMono->UnboxVector(secObj);
	CTransform* workTrans = CS_CompToComp<CTransform*>(obj); //TODO IMPORTANT: First parameter is the object reference, use that to find UID
	if (workTrans)
	{
		workTrans->SetPosition(omgItWorks);

	}
}

MonoObject* GetForward(MonoObject* go)
{
	if (External == nullptr)
		return nullptr;

	CTransform* trans = CS_CompToComp<CTransform*>(go);

	MonoClass* vecClass = mono_class_from_name(External->moduleMono->image, YMIR_SCRIPTS_NAMESPACE, "Vector3");
	return External->moduleMono->Float3ToCS(trans->GetForward());
}

MonoObject* GetRight(MonoObject* go)
{
	if (External == nullptr)
		return nullptr;

	CTransform* trans = CS_CompToComp<CTransform*>(go);

	MonoClass* vecClass = mono_class_from_name(External->moduleMono->image, YMIR_SCRIPTS_NAMESPACE, "Vector3");
	return External->moduleMono->Float3ToCS(trans->GetRight());
}

MonoObject* GetUp(MonoObject* go)
{	
	if (External == nullptr)
		return nullptr;

	CTransform* trans = CS_CompToComp<CTransform*>(go);
		
	MonoClass* vecClass = mono_class_from_name(External->moduleMono->image, YMIR_SCRIPTS_NAMESPACE, "Vector3");
	return External->moduleMono->Float3ToCS(trans->GetUp());
}

MonoObject* GetLocalForward(MonoObject* go)
{
	if (External == nullptr)
		return nullptr;

	CTransform* trans = CS_CompToComp<CTransform*>(go);

	MonoClass* vecClass = mono_class_from_name(External->moduleMono->image, YMIR_SCRIPTS_NAMESPACE, "Vector3");
	return External->moduleMono->Float3ToCS(trans->GetLocalForward());
}

MonoObject* GetLocalRight(MonoObject* go)
{
	if (External == nullptr)
		return nullptr;

	CTransform* trans = CS_CompToComp<CTransform*>(go);

	MonoClass* vecClass = mono_class_from_name(External->moduleMono->image, YMIR_SCRIPTS_NAMESPACE, "Vector3");
	return External->moduleMono->Float3ToCS(trans->GetLocalRight());
}

MonoObject* GetLocalUp(MonoObject* go)
{
	if (External == nullptr)
		return nullptr;

	CTransform* trans = CS_CompToComp<CTransform*>(go);

	MonoClass* vecClass = mono_class_from_name(External->moduleMono->image, YMIR_SCRIPTS_NAMESPACE, "Vector3");
	return External->moduleMono->Float3ToCS(trans->GetLocalUp());
}

MonoObject* SendRotation(MonoObject* obj) //Allows to send float3 as "objects" in C#, should find a way to move Vector3 as class
{
	return Ymir_Box_Quat(obj, false); //Use this method to send class types
}

void RecieveRotation(MonoObject* obj, MonoObject* secObj) //Allows to send float3 as "objects" in C#, should find a way to move Vector3 as class
{
	if (External == nullptr)
		return;

	Quat omgItWorks = External->moduleMono->UnboxQuat(secObj);
	CTransform* transform = CS_CompToComp<CTransform*>(obj); //TODO IMPORTANT: First parameter is the object reference, use that to find UID

	if (transform)
	{
		//workGO->transform->SetTransformMatrix(workGO->transform->position, omgItWorks, workGO->transform->localScale);

		transform->rotation = omgItWorks.Normalized();
		transform->eulerRot = omgItWorks.ToEulerXYZ() * RADTODEG;

		transform->dirty_ = true;
	}
}

MonoObject* SendScale(MonoObject* obj)
{
	return Ymir_Box_Vector(obj, "SCALE", false);
}

void RecieveScale(MonoObject* obj, MonoObject* secObj)
{
	if (External == nullptr)
		return;

	float3 omgItWorks = External->moduleMono->UnboxVector(secObj);
	GameObject* workGO = External->moduleMono->GameObject_From_CSGO(obj); //TODO IMPORTANT: First parameter is the object reference, use that to find UID

	if (workGO->mTransform)
	{
		//workGO->mTransform->SetTransformMatrix(workGO->mTransform->translation, workGO->mTransform->rotation, omgItWorks);
		//workGO->mTransform->updateTransform = true; //TODO: No tenemos la variable esta "updateTransform"
	}
}

void SetActive(MonoObject* obj, bool active)
{
	if (External == nullptr)
		return;

	GameObject* go = External->moduleMono->GameObject_From_CSGO(obj);
	go->active = active;
	External->scene->SetActiveRecursively(go, active);
}

void ExitGame()
{
	External->editor->exit = update_status::UPDATE_STOP;

	LOG("Exit game");
}

void LoadSceneCS(MonoString* scenePath)
{
	char* _path = mono_string_to_utf8(scenePath);
	External->scene->pendingToAddScene = _path;
}

void Destroy(MonoObject* go)
{
	if (go == NULL)
		return;

	MonoClass* klass = mono_object_get_class(go);
	//const char* name = mono_class_get_name(klass);

	GameObject* workGO = External->moduleMono->GameObject_From_CSGO(go);
	//GameObject* workGO = C_Script::runningScript->GetGO();
	if (workGO == nullptr) {
		LOG("[ERROR] Can't delete GameObject from CS");
		return;
	}

	workGO->DestroyGameObject();
}

float GetDT()
{
	//IDK if this would work
	return External->GetDT();
}

float GetTimeCS()
{
	return TimeManager::gameTimer.ReadSec();
}

void CreateBullet(MonoObject* position, MonoObject* rotation, MonoObject* scale)
{
	//Crea un game object temporal llamado "Bullet"
	if (External == nullptr) return;
	GameObject* go = External->scene->PostUpdateCreateGameObject("Bullet", External->scene->mRootNode);
	go->UID = Random::Generate();

	//Hace unbox de los parametros de transform pasados
	float3 posVector = External->moduleMono->UnboxVector(position);
	Quat rotVector = External->moduleMono->UnboxQuat(rotation);
	float3 scaleVector = External->moduleMono->UnboxVector(scale);

	//Settea el transform a la bullet
	go->mTransform->SetPosition(posVector);
	go->mTransform->rotation = rotVector.Normalized();
	go->mTransform->SetScale(scaleVector);

	uint UID = 1553236809; // UID of Cube.fbx mesh in meta (lo siento)

	std::string libraryPath = External->fileSystem->libraryMeshesPath + std::to_string(UID) + ".ymesh";

	//if (!PhysfsEncapsule::FileExists(libraryPath)) {

	//	External->resourceManager->ImportFile("Assets/Primitives/Cube.fbx", true);

	//}

	//Añade la mesh a la bullet
	ResourceMesh* rMesh = (ResourceMesh*)(External->resourceManager->CreateResourceFromLibrary(libraryPath, ResourceType::MESH, UID));
	CMesh* cmesh = new CMesh(go);
	cmesh->rMeshReference = rMesh;
	go->AddComponent(cmesh);

	//Añade el material a la Bullet
	CMaterial* cmaterial = new CMaterial(go);
	cmaterial->shaderPath = SHADER_VS_FS;
	cmaterial->shader.LoadShader(cmaterial->shaderPath);
	cmaterial->shaderDirtyFlag = false;
	go->AddComponent(cmaterial);

	//Añade RigidBody a la bala
	CCollider* physBody;
	physBody = new CCollider(go);
	physBody->useGravity = false;
	physBody->size = scaleVector;
	physBody->physBody->SetPosition(posVector);
	go->AddComponent(physBody);

	//Añade el script Bullet al gameObject Bullet
	const char* t = "BH_Bullet";
	Component* c = nullptr;
	c = new CScript(go, t);
	go->AddComponent(c);

}

void CreateTailSensor(MonoObject* position, MonoObject* rotation)
{
	//Crea un game object temporal llamado "Bullet"
	if (External == nullptr) return;
	GameObject* go = External->scene->PostUpdateCreateGameObject("Tail", External->scene->mRootNode);
	go->UID = Random::Generate();
	go->tag = "Tail";

	//Hace unbox de los parametros de transform pasados
	float3 posVector = External->moduleMono->UnboxVector(position);
	Quat rotVector = External->moduleMono->UnboxQuat(rotation);
	float3 scaleVector = float3(1, 1, 3);


	//Añade RigidBody a la bala
	CCollider* physBody;
	physBody = new CCollider(go);
	physBody->useGravity = false;
	physBody->physBody->SetPosition(posVector);
	physBody->physBody->SetRotation(rotVector.Normalized());
	physBody->SetAsSensor(true);

	go->AddComponent(physBody);
	physBody->physBody->body->activate(true);
	physBody->size = scaleVector;
	physBody->shape->setLocalScaling(btVector3(scaleVector.x, scaleVector.y, scaleVector.z));

	//Añade el script Tail al gameObject Bullet
	const char* t = "BH_Tail";
	Component* c = nullptr;
	c = new CScript(go, t);
	go->AddComponent(c);

}

//---------- GLOBAL GETTERS ----------//
MonoObject* SendGlobalPosition(MonoObject* obj) //Allows to send float3 as "objects" in C#, should find a way to move Vector3 as class
{
	//return mono_value_box(External->moduleMono->domain, vecClass, External->moduleMono->Float3ToCS(C_Script::runningScript->GetGO()->transform->position)); //Use this method to send "object" types
	return Ymir_Box_Vector(obj, "POSITION", true); //Use this method to send class types
}

MonoObject* SendGlobalRotation(MonoObject* obj) //Allows to send float3 as "objects" in C#, should find a way to move Vector3 as class
{
	//return mono_value_box(External->moduleMono->domain, vecClass, External->moduleMono->Float3ToCS(C_Script::runningScript->GetGO()->transform->position)); //Use this method to send "object" types
	return Ymir_Box_Quat(obj, true); //Use this method to send class types
}

MonoObject* SendGlobalScale(MonoObject* obj) //Allows to send float3 as "objects" in C#, should find a way to move Vector3 as class
{
	//return mono_value_box(External->moduleMono->domain, vecClass, External->moduleMono->Float3ToCS(C_Script::runningScript->GetGO()->transform->position)); //Use this method to send "object" types
	return Ymir_Box_Vector(obj, "SCALE", true); //Use this method to send class types
}

MonoString* GetTag(MonoObject* cs_Object)
{
	GameObject* cpp_gameObject = External->moduleMono->GameObject_From_CSGO(cs_Object);

	return mono_string_new(External->moduleMono->domain, cpp_gameObject->tag.c_str());
}

void SetTag(MonoObject* cs_Object, MonoString* string)
{
	GameObject* cpp_gameObject = External->moduleMono->GameObject_From_CSGO(cs_Object);

	std::string newTag = mono_string_to_utf8(string);
	std::vector<std::string> tags = External->scene->tags;

	bool tagAlreadyExists = false;
	for (int t = 0; t < tags.size(); t++)
	{
		if (strcmp(newTag.c_str(), tags[t].c_str()) == 0)
		{
			tagAlreadyExists = true;
			break;
		}
	}
	if (tagAlreadyExists == false)
	{
		External->scene->tags.push_back(newTag);
	}
	cpp_gameObject->tag = newTag.c_str();
}

MonoObject* CreateImageUI(MonoObject* pParent, MonoString* newImage, int x, int y)
{
	GameObject* ui_gameObject = External->moduleMono->GameObject_From_CSGO(pParent);
	std::string _newImage = mono_string_to_utf8(newImage);

	G_UI* tempGameObject = new G_UI(External->scene->mRootNode, 0, 0);

	tempGameObject->AddImage(_newImage, x, y, 100, 100);

	External->scene->PostUpdateCreateGameObject_UI((GameObject*)tempGameObject);

	return External->moduleMono->GoToCSGO(tempGameObject);
}

void Rumble_Controller(int time, int intenisty)
{
	if (External != nullptr) {

		intenisty = intenisty * 6500;
		if (SDL_JoystickRumble(External->input->joystick, intenisty, intenisty, time) == -1) {
			printf("Rumble failed...?\n");
		}
		else {
			printf("Rumble success!\n");
		}
	}
}

void ChangeImageUI(MonoObject* pParent, MonoString* newImage, MonoString* imageToChange, int x, int y)
{
	//Falta meter automaticamente que haga el change de Image
	GameObject* go_image_to_change = External->moduleMono->GameObject_From_CSGO(pParent);
	std::string _newImage = mono_string_to_utf8(newImage);
	std::string _findbyname = mono_string_to_utf8(imageToChange);

	for (auto it = External->scene->gameObjects.begin(); it != External->scene->gameObjects.end(); ++it)
	{
		if ((*it)->name == _findbyname)
		{
			UI_Image* image_to_change = static_cast<UI_Image*>(static_cast<G_UI*>((*it))->GetComponentUI(UI_TYPE::IMAGE));

			image_to_change->SetImg(_newImage, UI_STATE::NORMAL);
		}
	}
}

void TextEdit(MonoObject* object, MonoString* text)
{
	G_UI* go = (G_UI*)External->moduleMono->GameObject_From_CSGO(object);
	static_cast<UI_Text*>(go->GetComponentUI(UI_TYPE::TEXT))->SetText(mono_string_to_utf8(text));
}

void SliderEdit(MonoObject* object, double value)
{
	G_UI* go = (G_UI*)External->moduleMono->GameObject_From_CSGO(object);
	static_cast<UI_Slider*>(go->GetComponentUI(UI_TYPE::SLIDER))->SetValue(value);
}

void SliderSetRange(MonoObject* object, double min, double max)
{
	G_UI* go = (G_UI*)External->moduleMono->GameObject_From_CSGO(object);

	if (static_cast<UI_Slider*>(go->GetComponentUI(UI_TYPE::SLIDER))->useFloat)
	{
		static_cast<UI_Slider*>(go->GetComponentUI(UI_TYPE::SLIDER))->minValue.fValue = min;
		static_cast<UI_Slider*>(go->GetComponentUI(UI_TYPE::SLIDER))->maxValue.fValue = max;
	}
	else
	{
		static_cast<UI_Slider*>(go->GetComponentUI(UI_TYPE::SLIDER))->minValue.iValue = min;
		static_cast<UI_Slider*>(go->GetComponentUI(UI_TYPE::SLIDER))->maxValue.iValue = max;
	}
}

void SliderSetMin(MonoObject* object, double value)
{
	G_UI* go = (G_UI*)External->moduleMono->GameObject_From_CSGO(object);

	if (static_cast<UI_Slider*>(go->GetComponentUI(UI_TYPE::SLIDER))->useFloat)
	{
		static_cast<UI_Slider*>(go->GetComponentUI(UI_TYPE::SLIDER))->minValue.fValue = value;
	}
	else
	{
		static_cast<UI_Slider*>(go->GetComponentUI(UI_TYPE::SLIDER))->minValue.iValue = value;
	}
}

void SliderSetMax(MonoObject* object, double value)
{
	G_UI* go = (G_UI*)External->moduleMono->GameObject_From_CSGO(object);

	if (static_cast<UI_Slider*>(go->GetComponentUI(UI_TYPE::SLIDER))->useFloat)
	{
		static_cast<UI_Slider*>(go->GetComponentUI(UI_TYPE::SLIDER))->maxValue.fValue = value;
	}
	else
	{
		static_cast<UI_Slider*>(go->GetComponentUI(UI_TYPE::SLIDER))->maxValue.iValue = value;
	}
}
#pragma endregion