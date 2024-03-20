#include "CAudioListener.h"
#include "External/MathGeoLib/include/Math/float4x4.h"
#include "GameObject.h"
#include "CTransform.h"
#include "External/ImGui/imgui.h"
#include "Application.h"
#include "ModuleAudio.h"

#include "External/mmgr/mmgr.h"

CAudioListener::CAudioListener(GameObject* owner, bool defaultListener) :Component(owner,ComponentType::AUDIO_LISTENER), isDefaultListener(true), myTransform(nullptr)
{
	myTransform = dynamic_cast<CTransform*>(owner->GetComponent(ComponentType::TRANSFORM));

	id = static_cast<unsigned int>(Random::Generate());
	External->audio->RegisterNewAudioObject(id);
	SetAsDefaultListener(defaultListener);
}

CAudioListener::~CAudioListener()
{
	External->audio->UnRegisterAudioObject(id);
	SetAsDefaultListener(false);
	myTransform = nullptr;
}


void CAudioListener::OnInspector()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	bool exists = true;

	ImGui::Checkbox(("##" + std::to_string(UID)).c_str(), &active);
	ImGui::SameLine();


	if (ImGui::CollapsingHeader(("Audio Listener##" + std::to_string(UID)).c_str(), &exists, flags))
	{
		if (!active) { ImGui::BeginDisabled(); }

		/*ImGui::Text("AudioClip");
		ImGui::SameLine(ImGui::GetWindowWidth() * 0.65f);
		bool deflistener = isDefaultListener;
		ImGui::Checkbox("##AudioClip", &deflistener);
		ImGui::SameLine();
		ImGui::Text("Listen"); 
		
		ImGui::Separator();*/

		bool listenerAux = isDefaultListener;
		if (ImGui::Checkbox("Default Listener", &listenerAux))
		{
			SetAsDefaultListener(listenerAux);
		}

		if (!active) { ImGui::EndDisabled(); }
	}

	if (!exists) { mOwner->RemoveComponent(this); }
}

void CAudioListener::Update()
{
	float3 pos = myTransform->mGlobalMatrix.TranslatePart();
	External->audio->SetAudioObjTransform(id, pos, myTransform->GetForward(), myTransform->GetUp());
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

uint CAudioListener::GetID()
{
	return id;
}

void CAudioListener::SetID(uint id)
{
	External->audio->UnRegisterAudioObject(this->id);
	this->id = id;
	External->audio->RegisterNewAudioObject(id);
}

void CAudioListener::SetAsDefaultListener(bool setDefault)
{
	if (setDefault)
	{
		if (External->audio->defaultListener != nullptr)
		{
			if (External->audio->defaultListener != this)
			{
				External->audio->defaultListener->SetAsDefaultListener(false);
			}
		}
		External->audio->defaultListener = this;
		External->audio->WwiseListnerHasToUpdate();


	}
	else if (External->audio->defaultListener != nullptr)
	{
		if (External->audio->defaultListener == this)
		{
			External->audio->defaultListener = nullptr;
			External->audio->WwiseListnerHasToUpdate();
		}
	}

	isDefaultListener = setDefault;
	SetVolume(External->audio->masterVolume);
}

bool CAudioListener::IsDefaultListener()
{
	return isDefaultListener;
}
