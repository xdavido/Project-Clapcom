#include "CScript.h"
#include "External/ImGui/imgui.h"

#include "ModuleEditor.h"
#include "ModuleScene.h"

#include "GameObject.h"
#include "Application.h"
#include "CTransform.h"

//#include "DEJsonSupport.h"		//TODO: IDK si necesitaremos esto
#include <mono/metadata/class.h>
#include <mono/metadata/object.h>
#include <mono/metadata/debug-helpers.h>
#include <filesystem>

#include "External/mmgr/mmgr.h"

CScript* CScript::runningScript = nullptr;
CScript::CScript(GameObject* _gm, const char* scriptName) : Component(_gm, ComponentType::SCRIPT), noGCobject(0), updateMethod(nullptr), startMethod(nullptr), onClickButtonMethod(nullptr), onHoverButtonMethod(nullptr), isStarting(true)
{
	name = scriptName;
	//strcpy(name, scriptName);

	/*External->moduleMono->DebugAllMethods(YMIR_SCRIPTS_NAMESPACE, "GameObject", methods);*/
	LoadScriptData(name);
}

CScript::~CScript()
{
	if (CScript::runningScript == this)
		CScript::runningScript = nullptr;

	mono_gchandle_free(noGCobject);

	for (unsigned int i = 0; i < fields.size(); i++)
	{
		if (fields[i].type == MonoTypeEnum::MONO_TYPE_CLASS && fields[i].fiValue.goValue != nullptr && fields[i].fiValue.goValue->csReferences.size() != 0)
		{
			std::vector<SerializedField*>::iterator ptr = std::find(fields[i].fiValue.goValue->csReferences.begin(), fields[i].fiValue.goValue->csReferences.end(), &fields[i]);
			if (ptr != fields[i].fiValue.goValue->csReferences.end())
				fields[i].fiValue.goValue->csReferences.erase(ptr);

		}
	}

	methods.clear();
	fields.clear();
	//name.clear();

}

void CScript::Update()
{
	if (TimeManager::gameTimer.GetState() == TimerState::STOPPED || TimeManager::gameTimer.GetState() == TimerState::PAUSED || updateMethod == nullptr)
		return;

	CScript::runningScript = this; // I really think this is the peak of stupid code, but hey, it works, slow as hell but works.
	MonoObject* exec2 = nullptr;

	if (startMethod && isStarting) {
		mono_runtime_invoke(startMethod, mono_gchandle_get_target(noGCobject), NULL, &exec2);
		isStarting = false;
	}
	MonoObject* exec = nullptr;

	mono_runtime_invoke(updateMethod, mono_gchandle_get_target(noGCobject), NULL, &exec);  //Peta al hacer PLAY en el motor

	if (exec != nullptr)
	{
		if (strcmp(mono_class_get_name(mono_object_get_class(exec)), "NullReferenceException") == 0)
		{
			LOG("[WARNING] Null reference exception detected");
		}
		else
		{
			LOG("[ERROR] Something went wrong");
		}
	}
}

void CScript::ReloadComponent() {

	LoadScriptData(name);

}

void CScript::OnRecursiveUIDChange(std::map<uint, GameObject*> gameObjects)
{
	for (size_t i = 0; i < fields.size(); i++)
	{
		if (fields[i].type == MonoTypeEnum::MONO_TYPE_CLASS && strcmp(mono_type_get_name(mono_field_get_type(fields[i].field)), "YmirEngine.GameObject") == 0)
		{
			std::map<uint, GameObject*>::iterator gameObjectIt = gameObjects.find(fields[i].goUID);

			if (gameObjectIt != gameObjects.end())
			{
				if (External->scene->referenceMap.size() > 0)
					External->scene->referenceMap.erase(gameObjectIt->first);

				External->scene->AddToReferenceMap((uint)gameObjectIt->second->UID, &fields[i]);

				fields[i].fiValue.goValue = gameObjectIt->second;
				fields[i].goUID = (uint)gameObjectIt->second->UID;
			}
		}
	}
}

void CScript::OnInspector()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	bool exists = true;

	ImGui::Checkbox(("##" + std::to_string(UID)).c_str(), &active);
	ImGui::SameLine();

	if (ImGui::CollapsingHeader(("Script##" + std::to_string(UID)).c_str(), &exists, flags))
	{
		ImGui::Text("Script name: %s", name.c_str());
		if (!active) { ImGui::BeginDisabled(); }

		ImGui::Text("Scripting things");

		for (int i = 0; i < fields.size(); i++)
		{
			DropField(fields[i], "GameObject");
		}

		ImGui::Separator();

		for (int i = 0; i < methods.size(); i++)
		{
			ImGui::Text(methods[i].c_str());
		}

		if (!active) { ImGui::EndDisabled(); }
	}

	if (!exists) { mOwner->RemoveComponent(this); }
}

//
//void CScript::SaveData(JSON_Object* nObj)
//{
//	Component::SaveData(nObj);
//	DEJson::WriteString(nObj, "ScriptName", name.c_str());
//
//	for (int i = 0; i < fields.size(); i++)
//	{
//		switch (fields[i].type)
//		{
//		case MonoTypeEnum::MONO_TYPE_BOOLEAN:
//			mono_field_get_value(mono_gchandle_get_target(noGCobject), fields[i].field, &fields[i].fiValue.bValue);
//			DEJson::WriteBool(nObj, mono_field_get_name(fields[i].field), fields[i].fiValue.bValue);
//			break;
//
//		case MonoTypeEnum::MONO_TYPE_I4:
//			mono_field_get_value(mono_gchandle_get_target(noGCobject), fields[i].field, &fields[i].fiValue.iValue);
//			DEJson::WriteInt(nObj, mono_field_get_name(fields[i].field), fields[i].fiValue.iValue);
//			break;
//
//		case MonoTypeEnum::MONO_TYPE_CLASS:
//			if (fields[i].fiValue.goValue != nullptr)
//				DEJson::WriteInt(nObj, mono_field_get_name(fields[i].field), fields[i].fiValue.goValue->UID);
//			break;
//
//		case MonoTypeEnum::MONO_TYPE_R4:
//			mono_field_get_value(mono_gchandle_get_target(noGCobject), fields[i].field, &fields[i].fiValue.fValue);
//			DEJson::WriteFloat(nObj, mono_field_get_name(fields[i].field), fields[i].fiValue.fValue);
//			break;
//
//		case MonoTypeEnum::MONO_TYPE_STRING:
//			DEJson::WriteString(nObj, mono_field_get_name(fields[i].field), fields[i].fiValue.strValue);
//			break;
//
//		default:
//			DEJson::WriteInt(nObj, mono_field_get_name(fields[i].field), fields[i].fiValue.iValue);
//			break;
//		}
//	}
//}
//void CScript::LoadData(DEConfig& nObj)
//{
//	Component::LoadData(nObj);
//
//	SerializedField* _field = nullptr;
//	for (int i = 0; i < fields.size(); i++) //TODO IMPORTANT ASK: There must be a better way to do this... too much use of switches with this stuff, look at MONOMANAGER
//	{
//		_field = &fields[i];
//
//		switch (_field->type)
//		{
//		case MonoTypeEnum::MONO_TYPE_BOOLEAN:
//			_field->fiValue.bValue = nObj.ReadBool(mono_field_get_name(_field->field));
//			mono_field_set_value(mono_gchandle_get_target(noGCobject), _field->field, &_field->fiValue.bValue);
//			break;
//
//		case MonoTypeEnum::MONO_TYPE_I4:
//			_field->fiValue.iValue = nObj.ReadInt(mono_field_get_name(_field->field));
//			mono_field_set_value(mono_gchandle_get_target(noGCobject), _field->field, &_field->fiValue.iValue);
//			break;
//
//		case MonoTypeEnum::MONO_TYPE_CLASS:
//		{
//			if (strcmp(mono_type_get_name(mono_field_get_type(_field->field)), "YmirEngine.GameObject") == 0)
//				EngineExternal->moduleScene->referenceMap.emplace(nObj.ReadInt(mono_field_get_name(_field->field)), _field);
//
//			break;
//		}
//		case MonoTypeEnum::MONO_TYPE_R4:
//			_field->fiValue.fValue = nObj.ReadFloat(mono_field_get_name(_field->field));
//			mono_field_set_value(mono_gchandle_get_target(noGCobject), _field->field, &_field->fiValue.fValue);
//			break;
//
//		case MonoTypeEnum::MONO_TYPE_STRING:
//		{
//			const char* ret = nObj.ReadString(mono_field_get_name(_field->field));
//
//			if (ret == NULL)
//				ret = "\0";
//
//			strcpy(&_field->fiValue.strValue[0], ret);
//
//			MonoString* str = mono_string_new(EngineExternal->moduleMono->domain, _field->fiValue.strValue);
//			mono_field_set_value(mono_gchandle_get_target(noGCobject), _field->field, str);
//			break;
//		}
//
//		default:
//			_field->fiValue.iValue = nObj.ReadInt(mono_field_get_name(_field->field));
//			mono_field_set_value(mono_gchandle_get_target(noGCobject), _field->field, &_field->fiValue.iValue);
//			break;
//		}
//	}
//}

void CScript::DropField(SerializedField& field, const char* dropType)
{

	const char* fieldName = mono_field_get_name(field.field);
	ImGui::PushID(fieldName);

	ImGui::Text(fieldName);
	ImGui::SameLine();

	switch (field.type)
	{
	case MonoTypeEnum::MONO_TYPE_BOOLEAN:
		mono_field_get_value(mono_gchandle_get_target(noGCobject), field.field, &field.fiValue.bValue);
		if (ImGui::Checkbox(field.displayName.c_str(), &field.fiValue.bValue))
			mono_field_set_value(mono_gchandle_get_target(noGCobject), field.field, &field.fiValue.bValue);
		break;

	case MonoTypeEnum::MONO_TYPE_I4:
		mono_field_get_value(mono_gchandle_get_target(noGCobject), field.field, &field.fiValue.iValue);
		if (ImGui::InputInt(field.displayName.c_str(), &field.fiValue.iValue, 1, 10))
			mono_field_set_value(mono_gchandle_get_target(noGCobject), field.field, &field.fiValue.iValue);
		break;

	case MonoTypeEnum::MONO_TYPE_CLASS:

		if (strcmp(mono_type_get_name(mono_field_get_type(field.field)), "YmirEngine.GameObject") != 0)
		{
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "The class %s can't be serialized yet", mono_type_get_name(mono_field_get_type(field.field)));
			break;
		}

		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), (field.fiValue.goValue != nullptr) ? field.fiValue.goValue->name.c_str() : "this");


		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(dropType))
			{
				if (field.fiValue.goValue != nullptr)
					field.fiValue.goValue->RemoveCSReference(&field);

				GameObject* draggedObject = External->editor->draggedGO;
				field.fiValue.goValue = draggedObject;
				field.goUID = draggedObject->UID;

				SetField(field.field, field.fiValue.goValue);
			}
			ImGui::EndDragDropTarget();
		}

		//Hardcodeado para que asigne el GO del objeto del script a todos los campos

		//field.fiValue.goValue = mOwner;
		//SetField(field.field, field.fiValue.goValue);

		//if (ImGui::BeginDragDropTarget())				//TODO: Esto se encarga de hager Drag & Drop el elemento reference
		//{
		///*	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(dropType))
		//	{
		//		field.fiValue.goValue = External->editor->hoveredGO;
		//		SetField(field.field, field.fiValue.goValue);
		//	}*/
		//	ImGui::EndDragDropTarget();
		//}
		break;

	case MonoTypeEnum::MONO_TYPE_R4: {
		//float test = 0.f;
		mono_field_get_value(mono_gchandle_get_target(noGCobject), field.field, &field.fiValue.fValue);
		if (ImGui::InputFloat(field.displayName.c_str(), &field.fiValue.fValue, 0.1f))
			mono_field_set_value(mono_gchandle_get_target(noGCobject), field.field, &field.fiValue.fValue);
		break;
	}

	case MonoTypeEnum::MONO_TYPE_STRING:
	{
		MonoString* str = nullptr;
		mono_field_get_value(mono_gchandle_get_target(noGCobject), field.field, &str);

		char* value = mono_string_to_utf8(str);
		strcpy(field.fiValue.strValue, value);
		mono_free(value);

		if (ImGui::InputText(field.displayName.c_str(), &field.fiValue.strValue[0], 50))
		{
			str = mono_string_new(External->moduleMono->domain, field.fiValue.strValue);
			mono_field_set_value(mono_gchandle_get_target(noGCobject), field.field, str);
			//mono_free(str);
		}
		break;
	}

	default:
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), mono_type_get_name(mono_field_get_type(field.field)));
		break;
	}
	//TODO: Update C# field value
	//ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), mono_type_get_name(mono_field_get_type(field.field)));
	ImGui::PopID();
}

void CScript::LoadScriptData(std::string scriptName)
{
	methods.clear();
	fields.clear();

	scriptName.find_first_of('.');
	size_t pos = scriptName.find_last_of('.');

	if (pos != std::string::npos) {
		scriptName = scriptName.substr(0, pos);
	}

	MonoClass* klass = mono_class_from_name(External->moduleMono->image, USER_SCRIPTS_NAMESPACE, scriptName.c_str());

	if (klass == nullptr)
	{
		LOG("Script %s was deleted and can't be loaded", scriptName);
		name = "Missing script reference";
		return;
	}

	External->moduleMono->DebugAllMethods(USER_SCRIPTS_NAMESPACE, scriptName.c_str(), methods);

	noGCobject = mono_gchandle_new(mono_object_new(External->moduleMono->domain, klass), false);
	mono_runtime_object_init(mono_gchandle_get_target(noGCobject));

	MonoClass* goClass = mono_object_get_class(mono_gchandle_get_target(noGCobject));
	uintptr_t ptr = reinterpret_cast<uintptr_t>(this);
	mono_field_set_value(mono_gchandle_get_target(noGCobject), mono_class_get_field_from_name(goClass, "pointer"), &ptr);

	MonoMethodDesc* mdesc = mono_method_desc_new(":Update", false);
	updateMethod = mono_method_desc_search_in_class(mdesc, klass);
	mono_method_desc_free(mdesc);

	MonoMethodDesc* oncDesc = mono_method_desc_new(":OnCollisionStay", false);
	onCollisionStayMethod = mono_method_desc_search_in_class(oncDesc, klass);
	mono_method_desc_free(oncDesc);

	oncDesc = mono_method_desc_new(":OnCollisionEnter", false);
	onCollisionEnterMethod = mono_method_desc_search_in_class(oncDesc, klass);
	mono_method_desc_free(oncDesc);

	oncDesc = mono_method_desc_new(":OnCollisionExit", false);
	onCollisionExitMethod = mono_method_desc_search_in_class(oncDesc, klass);
	mono_method_desc_free(oncDesc);

	MonoMethodDesc* oncBut = mono_method_desc_new(":OnClickButton", false);
	onClickButtonMethod = mono_method_desc_search_in_class(oncBut, klass);
	mono_method_desc_free(oncBut);

	oncDesc = mono_method_desc_new(":Start", false);
	startMethod = mono_method_desc_search_in_class(oncDesc, klass);
	mono_method_desc_free(oncDesc);

	
	MonoMethodDesc* onhBut = mono_method_desc_new(":OnHoverButton", false);
	onHoverButtonMethod = mono_method_desc_search_in_class(onhBut, klass);
	mono_method_desc_free(onhBut);

	oncDesc = mono_method_desc_new(":Start", false);
	startMethod = mono_method_desc_search_in_class(oncDesc, klass);
	mono_method_desc_free(oncDesc);


	MonoClass* baseClass = mono_class_get_parent(klass);
	if (baseClass != nullptr)
		External->moduleMono->DebugAllFields(mono_class_get_name(baseClass), fields, mono_gchandle_get_target(noGCobject), this, mono_class_get_namespace(baseClass));

	External->moduleMono->DebugAllFields(scriptName.c_str(), fields, mono_gchandle_get_target(noGCobject), this, mono_class_get_namespace(goClass));
}

void CScript::CollisionStayCallback(bool isTrigger, GameObject* collidedGameObject)
{
	void* params[1];

	if (collidedGameObject != nullptr)
	{
		params[0] = External->moduleMono->GoToCSGO(collidedGameObject);

		if (onCollisionStayMethod != nullptr)
			mono_runtime_invoke(onCollisionStayMethod, mono_gchandle_get_target(noGCobject), params, NULL);

		if (isTrigger)
		{
			if (onTriggerEnterMethod != nullptr)
				mono_runtime_invoke(onCollisionStayMethod, mono_gchandle_get_target(noGCobject), params, NULL);
		}
	}
}

void CScript::CollisionEnterCallback(bool isTrigger, GameObject* collidedGameObject)
{
	void* params[1];
	if (collidedGameObject != nullptr)
	{
		params[0] = External->moduleMono->GoToCSGO(collidedGameObject);

		if (onCollisionEnterMethod != nullptr)
			mono_runtime_invoke(onCollisionEnterMethod, mono_gchandle_get_target(noGCobject), params, NULL);

		if (isTrigger)
		{
			if (onTriggerEnterMethod != nullptr)
				mono_runtime_invoke(onCollisionEnterMethod, mono_gchandle_get_target(noGCobject), params, NULL);
		}
	}
}

void CScript::CollisionExitCallback(bool isTrigger, GameObject* collidedGameObject)
{
	void* params[1];
	if (collidedGameObject != nullptr)
	{
		params[0] = External->moduleMono->GoToCSGO(collidedGameObject);

		if (onCollisionExitMethod != nullptr)
		{
			mono_runtime_invoke(onCollisionExitMethod, mono_gchandle_get_target(noGCobject), params, NULL);
			External->physics->firstCollision = true; // Restablecer firstCollision aquí después de salir de la colisión
			External->physics->onExitCollision = false;
		}
			
			

		if (isTrigger)
		{
			if (onCollisionExitMethod != nullptr)
				mono_runtime_invoke(onCollisionExitMethod, mono_gchandle_get_target(noGCobject), params, NULL);
		}
	}
}

void CScript::OnClickButton() {

	if (onClickButtonMethod != nullptr) {

		mono_runtime_invoke(onClickButtonMethod, mono_gchandle_get_target(noGCobject), NULL, NULL);
	}
}

void CScript::OnHoverButton() {

	if (onHoverButtonMethod != nullptr) {

		mono_runtime_invoke(onHoverButtonMethod, mono_gchandle_get_target(noGCobject), NULL, NULL);
	}
}

void CScript::SetField(MonoClassField* field, GameObject* value)
{
	mono_field_set_value(mono_gchandle_get_target(noGCobject), field, External->moduleMono->GoToCSGO(value));
}
