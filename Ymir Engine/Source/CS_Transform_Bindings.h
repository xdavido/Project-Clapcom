#pragma once

#include "Globals.h"
#include "Application.h"

#include"GameObject.h"
#include"ResourceMesh.h"

#include"CMesh.h"
#include"CScript.h"
#include"CTransform.h"

#include"ModuleInput.h"
#include"ModuleScene.h"
#include"ModuleResourceManager.h"

#include"GameObject.h"
#include"MathGeoLib/include/Math/float3.h"

//------//
MonoObject* Ymir_Box_Vector(MonoObject* obj, const char* type, bool global)	//Retorna la nueva posición del objeto
{
	if (External == nullptr)
		return nullptr;

	const char* name = mono_class_get_name(mono_object_get_class(obj));

	float3 value;
	GameObject* workGO = External->moduleMono->GameObject_From_CSGO(obj);

	if (strcmp(type, "POSITION") == 0)
	{
		(global == true) ? value = workGO->mTransform->mGlobalMatrix.TranslatePart() : value = workGO->mTransform->translation;
	}
	else
	{
		(global == true) ? value = workGO->mTransform->mGlobalMatrix.GetScale() : value = workGO->mTransform->scale;
	}

	return External->moduleMono->Float3ToCS(value);
}
MonoObject* Ymir_Box_Quat(MonoObject* obj, bool global)	//Retorna la nueva rotación del objeto
{
	if (External == nullptr)
		return nullptr;

	const char* name = mono_class_get_name(mono_object_get_class(obj));

	Quat value;
	GameObject* workGO = External->moduleMono->GameObject_From_CSGO(obj);

	Quat qTmp = Quat::FromEulerXYZ(workGO->mTransform->rotation[0] * DEGTORAD, workGO->mTransform->rotation[1] * DEGTORAD, workGO->mTransform->rotation[2] * DEGTORAD);

	(global == true) ? value = workGO->mTransform->mGlobalMatrix.RotatePart().ToQuat().Normalized() : value = qTmp;


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
	GameObject* go = External->scene->CreateGameObject(p, External->scene->mRootNode);
	mono_free(p);

	float3 posVector = ModuleMonoManager::UnboxVector(position);

	go->mTransform->translation = posVector;
	//go->mTransform->updateTransform = true;	//TODO: No tenemos la variable esta "updateTransform"
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
	GameObject* workGO = External->moduleMono->GameObject_From_CSGO(obj); //TODO IMPORTANT: First parameter is the object reference, use that to find UID

	if (workGO->mTransform)
	{
		//workGO->mTransform->SetPosition(omgItWorks, workGO->mTransform->rotation, workGO->mTransform);
		//workGO->mTransform->updateTransform = true; //TODO: No tenemos la variable esta "updateTransform"
	}
}

//MonoObject* GetForward(MonoObject* go)	
//{
//	if (External == nullptr || CScript::runningScript == nullptr)
//		return nullptr;
//
//	GameObject* workGO = External->moduleMono->GameObject_From_CSGO(go);
//
//	MonoClass* vecClass = mono_class_from_name(External->moduleMono->image, DE_SCRIPTS_NAMESPACE, "Vector3");
//
//	return External->moduleMono->Float3ToCS(workGO->mTransform->GetForward());	//TODO: No tenemos GetForward()
//}
//MonoObject* GetRight(MonoObject* go)
//{
//	if (External == nullptr)
//		return nullptr;
//
//	GameObject* workGO = External->moduleMono->GameObject_From_CSGO(go);
//
//	MonoClass* vecClass = mono_class_from_name(External->moduleMono->image, DE_SCRIPTS_NAMESPACE, "Vector3");
//	return External->moduleMono->Float3ToCS(workGO->mTransform->GetRight());	//TODO: No tenemos GetRight()
//}

MonoObject* SendRotation(MonoObject* obj) //Allows to send float3 as "objects" in C#, should find a way to move Vector3 as class
{
	return Ymir_Box_Quat(obj, false); //Use this method to send class types
}
void RecieveRotation(MonoObject* obj, MonoObject* secObj) //Allows to send float3 as "objects" in C#, should find a way to move Vector3 as class
{
	if (External == nullptr)
		return;

	Quat omgItWorks = External->moduleMono->UnboxQuat(secObj);
	GameObject* workGO = External->moduleMono->GameObject_From_CSGO(obj); //TODO IMPORTANT: First parameter is the object reference, use that to find UID

	if (workGO->mTransform)
	{
		//workGO->mTransform->SetPosition(workGO->mTransform->translation, omgItWorks, workGO->mTransform->localScale);
		//workGO->mTransform->updateTransform = true; //TODO: No tenemos la variable esta "updateTransform"
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

void Destroy(MonoObject* go)
{
	if (go == NULL)
		return;

	MonoClass* klass = mono_object_get_class(go);
	//const char* name = mono_class_get_name(klass);

	GameObject* workGO = External->moduleMono->GameObject_From_CSGO(go);
	//GameObject* workGO = C_Script::runningScript->GetGO();
	if (workGO == nullptr) {
		LOG("AY LMAO CANT DELETE AYAYAYAYTA");		//Que le pasa al Miquel?
		return;
	}

	workGO->DestroyGameObject();
}

float GetDT()
{
	//IDK if this would work
	return External->GetDT();
}

//TODO:
//void CreateBullet(MonoObject* position, MonoObject* rotation, MonoObject* scale) //TODO: We really need prefabs
//{
//	if (External == nullptr)
//		return /*nullptr*/;
//
//	GameObject* go = External->scene->CreateGameObject("Empty", External->scene->root);
//	////go->name = std::to_string(go->UID);
//
//	float3 posVector = ModuleMonoManager::UnboxVector(position);
//	Quat rotQuat = ModuleMonoManager::UnboxQuat(rotation);
//	float3 scaleVector = ModuleMonoManager::UnboxVector(scale);
//
//	go->mTransform->SetTransformMatrix(posVector, rotQuat, scaleVector);
//	//go->mTransform->updateTransform = true; //TODO: No temenos esta variable "updateTransform"
//
//
//	CMesh* meshRenderer = dynamic_cast<CMesh*>(go->AddComponent(ComponentType::MESH));	//TODO: Crear un componente de tipo mesh para añadirle al game object
//
//	ResourceMesh* test = dynamic_cast<ResourceMesh*>(External->moduleResources->RequestResource(1736836885, "Library/Meshes/1736836885.mmh"));
//	meshRenderer->SetRenderMesh(test);
//
//	go->AddComponent(Component::Type::Script, "BH_Bullet");	//TODO: Añadir el componente script del Bullet al GameObject
//
//	/*return mono_gchandle_get_target(cmp->noGCobject);*/
//}

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

#pragma endregion