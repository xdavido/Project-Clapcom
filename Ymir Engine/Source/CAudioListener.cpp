#include "CAudioListener.h"
#include "External/MathGeoLib/include/Math/float4x4.h"
#include "GameObject.h"
#include "CTransform.h"
#include "External/ImGui/imgui.h"
#include "Application.h"
#include "ModuleAudio.h"

CAudioListener::CAudioListener(GameObject* _gm, bool defaultListener) :Component(_gm), isDefaultListener(false), myTransform(nullptr)
{
	
	myTransform = dynamic_cast<Cransform*>(gameObject->GetComponent(Component::TYPE::TRANSFORM));

	id = static_cast<unsigned int>(EngineExternal->GetRandomInt());
	EngineExternal->moduleAudio->RegisterNewAudioObject(id);
	SetAsDefaultListener(defaultListener);
}


CAudioListener::~CAudioListener()
{
	EngineExternal->moduleAudio->UnRegisterAudioObject(id);
	SetAsDefaultListener(false);
	myTransform = nullptr;
}

#ifndef STANDALONE
bool CAudioListener::OnEditor()
{
	if (Component::OnEditor() == true)
	{
		ImGui::Separator();

		bool listenerAux = isDefaultListener;
		if (ImGui::Checkbox("Default Listener", &listenerAux))
		{
			SetAsDefaultListener(listenerAux);
		}

		return true;
	}
	return false;
}
#endif // !STANDALONE

void CAudioListener::Update()
{
	float3 pos = myTransform->mGlobalMatrix.TranslatePart();
	float3 axis = { 1,0,0 };
	float3 axis2 = { 1,0,0 };
	External->audio->SetAudioObjTransform(id, pos, axis, axis2);
}
//
//void CAudioListener::SaveData(JSON_Object* nObj)
//{
//	Component::SaveData(nObj);
//	DEJson::WriteBool(nObj, "isDefaultListener", isDefaultListener);
//
//}
//
//void CAudioListener::LoadData(JSON_Object& nObj)
//{
//	Component::LoadData(nObj);
//	SetAsDefaultListener(nObj.ReadBool("isDefaultListener"));
//
//}



void CAudioListener::SetVolume(float newVol)
{
	newVol = MIN(newVol, 99.99f);
	newVol = MAX(newVol, 0.0f);

	if (isDefaultListener)
		External->audio->SetBusVolume(newVol);
}
//
//uint C_AudioListener::GetID()
//{
//	return id;
//}
//


CAudioListener::CAudioListener(GameObject* owner, bool defaultListener)
{

}

CAudioListener::~CAudioListener()
{

}

void CAudioListener::OnInspector()
{

}

void CAudioListener::Update()
{

}

void CAudioListener::SetVolume(float newVol)
{

}

uint CAudioListener::GetID()
{
	return id;
}

void CAudioListener::SetID(uint id)
{
	//	EngineExternal->moduleAudio->UnRegisterAudioObject(this->id);
//	this->id = id;
//	EngineExternal->moduleAudio->RegisterNewAudioObject(id);
}

void CAudioListener::SetAsDefaultListener(bool setDefault)
{
	//	if (setDefault)
//	{
//		if (EngineExternal->moduleAudio->defaultListener != nullptr)
//		{
//			if (EngineExternal->moduleAudio->defaultListener != this)
//			{
//				EngineExternal->moduleAudio->defaultListener->SetAsDefaultListener(false);
//			}
//
//		}
//		EngineExternal->moduleAudio->defaultListener = this;
//		EngineExternal->moduleAudio->WwiseListnerHasToUpdate();
//
//
//	}
//	else if (EngineExternal->moduleAudio->defaultListener != nullptr)
//	{
//		if (EngineExternal->moduleAudio->defaultListener == this)
//		{
//			EngineExternal->moduleAudio->defaultListener = nullptr;
//			EngineExternal->moduleAudio->WwiseListnerHasToUpdate();
//		}
//	}
//
//	isDefaultListener = setDefault;
//	SetVolume(EngineExternal->moduleAudio->masterVolume);
}

bool CAudioListener::IsDefaultListener()
{
	return isDefaultListener;
}
