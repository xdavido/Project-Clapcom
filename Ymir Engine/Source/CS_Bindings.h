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



template<typename T>
T DECS_CompToComp(MonoObject* obj)
{
	uintptr_t ptr = 0;
	MonoClass* goClass = mono_object_get_class(obj);
	const char* name = mono_class_get_name(goClass);

	mono_field_get_value(obj, mono_class_get_field_from_name(goClass, "pointer"), &ptr);

	return reinterpret_cast<T>(ptr);
}
//------//
MonoObject* Ymir_Box_Vector(MonoObject* obj, const char* type, bool global)	//Retorna la nueva posición del objeto
{
	//TODO: Quitar esto mas adelante, cuando esté arreglado el Transform
	
		
	if (External == nullptr)
		return nullptr;

	const char* name = mono_class_get_name(mono_object_get_class(obj));

	float3 value;
	CTransform* workTrans = DECS_CompToComp<CTransform*>(obj);

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
MonoObject* Ymir_Box_Quat(MonoObject* obj, bool global)	//Retorna la nueva rotación del objeto
{
	//TODO: Quitar esto mas adelante, cuando esté arreglado el Transform
	return nullptr;

	if (External == nullptr)
		return nullptr;

	const char* name = mono_class_get_name(mono_object_get_class(obj));

	Quat value	;
	GameObject* workGO = External->moduleMono->GameObject_From_CSGO(obj);

	Quat qTmp = Quat::FromEulerXYZ(workGO->mTransform->rotation.x * DEGTORAD, workGO->mTransform->rotation.y * DEGTORAD, workGO->mTransform->rotation.z * DEGTORAD);

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
GameObject* DECS_Comp_To_GameObject(MonoObject* component)
{
	uintptr_t ptr = 0;
	MonoClass* goClass = mono_object_get_class(component);

		mono_field_get_value(component, mono_class_get_field_from_name(goClass, "pointer"), &ptr);

	return reinterpret_cast<Component*>(ptr)->mOwner;
}
MonoObject* CS_Component_Get_GO(MonoObject* thisRef)
{	
	return External->moduleMono->GoToCSGO(DECS_Comp_To_GameObject(thisRef));
}
MonoString* Get_GO_Name(MonoObject* go)
{
	if (External == nullptr)
		return nullptr;

	return mono_string_new(
		External->moduleMono->domain,
		External->moduleMono->GameObject_From_CSGO(go)->name.c_str());
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
	CTransform* workTrans = DECS_CompToComp<CTransform*>(obj); //TODO IMPORTANT: First parameter is the object reference, use that to find UID
	if (workTrans)
	{
		workTrans->translation = omgItWorks;
	
	}
}

MonoObject* GetForward(MonoObject* go)	
{
	if (External == nullptr || CScript::runningScript == nullptr)
		return nullptr;

	GameObject* workGO = External->moduleMono->GameObject_From_CSGO(go);

	MonoClass* vecClass = mono_class_from_name(External->moduleMono->image, YMIR_SCRIPTS_NAMESPACE, "Vector3");

	return External->moduleMono->Float3ToCS(workGO->mTransform->GetForward());	//TODO: No tenemos GetForward()
}
MonoObject* GetRight(MonoObject* go)
{
	if (External == nullptr)
		return nullptr;

	GameObject* workGO = External->moduleMono->GameObject_From_CSGO(go);

	MonoClass* vecClass = mono_class_from_name(External->moduleMono->image, YMIR_SCRIPTS_NAMESPACE, "Vector3");
	return External->moduleMono->Float3ToCS(workGO->mTransform->GetRight());	//TODO: No tenemos GetRight()
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

MonoString* GetTag(MonoObject* cs_Object)
{
	GameObject* cpp_gameObject = External->moduleMono->GameObject_From_CSGO(cs_Object);

	return mono_string_new(External->moduleMono->domain, cpp_gameObject->tag);
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
	strcpy(cpp_gameObject->tag, newTag.c_str());
}

#pragma endregion