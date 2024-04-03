#include "CAudioSource.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "GameObject.h"
#include "CTransform.h"
#include "Globals.h"
#include "External/ImGui/imgui.h"
#include "Log.h"

#include "ModuleScene.h"
#include "ModuleInput.h"

#include "External/mmgr/mmgr.h"

CAudioSource::CAudioSource(GameObject* owner) : Component(owner, ComponentType::AUDIO_SOURCE), audBankReference(nullptr), evName(""), isMuted(false), pitch(50.0f), playOnAwake(false), volume(50.0f), audBankName(""), isMusic(false)
{
	id = static_cast<unsigned int>(Random::Generate());
	gameObjectTransform = dynamic_cast<CTransform*>(owner->GetComponent(ComponentType::TRANSFORM));
	External->audio->RegisterNewAudioObject(id);
	External->audio->AddAudioSource(this);

	//External->audio->LoadBank(std::string("Music"));
	//External->audio->PlayEvent(this->id, std::string("Music"));


	evID = 0;
}

CAudioSource::~CAudioSource()
{
	if (isMusic) {
		for (std::vector<CAudioSource*>::iterator it = External->audio->musicSource.begin(); it != External->audio->musicSource.end(); ++it) {
			if ((*it) == this) {
				External->audio->musicSource.erase(it);
				break;
			}
		}
	}

	External->audio->StopComponent(id);
	External->audio->RemoveAudioSource(this);
	External->audio->UnRegisterAudioObject(id);

	gameObjectTransform = nullptr;
	audBankReference = nullptr;

}

#ifndef STANDALONE
void CAudioSource::OnInspector()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

	bool exists = true;

	ImGui::Checkbox(("##" + std::to_string(UID)).c_str(), &active);
	ImGui::SameLine();

	if (ImGui::CollapsingHeader(("Audio Source##" + std::to_string(UID)).c_str(), &exists, flags))
	{
		if (!active) { ImGui::BeginDisabled(); }

		if (ImGui::BeginCombo("Audio Bank", audBankName.c_str()))
		{
			std::vector<AudioBank*>::const_iterator it;
			for (it = External->audio->banks.begin(); it != External->audio->banks.end(); ++it)
			{
				bool isSelected = (audBankName == (*it)->bank_name);
				if (ImGui::Selectable((*it)->bank_name.c_str()))
				{
					audBankReference = (*it);
					audBankName = (*it)->bank_name;
					if (!(*it)->loaded_in_heap)
					{
						(*it)->loaded_in_heap = External->audio->LoadBank(audBankReference->bank_name);
					}
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (audBankReference != nullptr && ImGui::BeginCombo("Audio to Play", evName.c_str()))
		{
			std::map<uint64, std::string>::const_iterator ev_it;
			for (ev_it = audBankReference->actions.begin(); ev_it != audBankReference->actions.end(); ++ev_it)
			{
				bool isSelected = (evName == (*ev_it).second);
				if (ImGui::Selectable((*ev_it).second.c_str()))
				{
					evName = (*ev_it).second;
					evID = (*ev_it).first;
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("Play"))
		{
			PlayEvent();
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			StopEvent(evName);
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop All"))
		{
			StopEvent();
		}

		ImGui::Checkbox("Play on Awake", &playOnAwake);

		if (ImGui::Checkbox("Music", &isMusic)) {
			if (isMusic) {
				External->audio->musicSource.push_back(this);
			}
			else {
				for (std::vector<CAudioSource*>::iterator it = External->audio->musicSource.begin(); it != External->audio->musicSource.end(); ++it) {
					if ((*it) == this) {
						External->audio->musicSource.erase(it);
						break;
					}
				}
			}
		}

		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 99.99f))
		{
			SetVolume(volume);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Mute", &isMuted))
		{
			SetMuted(isMuted);
		}

		if (ImGui::SliderFloat("Pitch", &pitch, 0.0f, 100.0f))
		{
			SetPitch(pitch);
		}

		if (!active) { ImGui::EndDisabled(); }
	}

	if (!exists) { mOwner->RemoveComponent(this); }
}
#endif // !STANDALONE

void CAudioSource::Update()
{
	float3 pos = gameObjectTransform->mGlobalMatrix.TranslatePart();
	External->audio->SetAudioObjTransform(this->id, pos, gameObjectTransform->GetForward(), gameObjectTransform->GetUp());

	//if (External->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	//{
	//	External->audio->PlayEvent(this->id, std::string("Music"));
	//}
	//if (External->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
	//{
	//	External->audio->StopEvent(this->id, std::string("Music"));
	//}
}

//void CAudioSource::SaveData(JSON_Object* nObj)
//{
//	Component::SaveData(nObj);
//	DEJson::WriteString(nObj, "evName", this->evName.c_str());
//
//	if (this->audBankReference != nullptr)
//		DEJson::WriteString(nObj, "audBankReference", this->audBankReference->bank_name.c_str());
//
//	DEJson::WriteFloat(nObj, "pitch", this->pitch);
//	DEJson::WriteBool(nObj, "playOnAwake", this->playOnAwake);
//	DEJson::WriteBool(nObj, "isMuted", this->isMuted);
//	DEJson::WriteBool(nObj, "isMusic", this->isMusic);
//}
//
//void CAudioSource::LoadData(DEConfig& nObj)
//{
//	Component::LoadData(nObj);
//
//	this->evName = nObj.ReadString("evName");
//	std::string bankName = nObj.ReadString("audBankReference");
//	SetMuted(nObj.ReadBool("isMuted"));
//	SetPitch(nObj.ReadFloat("pitch"));
//	this->playOnAwake = nObj.ReadBool("playOnAwake");
//	isMusic = nObj.ReadBool("isMusic");
//	if (isMusic) {
//		EngineExternal->moduleAudio->musicSource.push_back(this);
//	}
//
//	// Iterate and assign audio bank. If not loaded, load
//	std::vector<AudioBank*>::iterator it;
//	for (it = EngineExternal->moduleAudio->banks.begin(); it != EngineExternal->moduleAudio->banks.end(); ++it)
//	{
//		if ((*it)->bank_name == bankName)
//		{
//			audBankReference = (*it);
//			audBankName = bankName;
//
//			if (!(*it)->loaded_in_heap)
//			{
//				EngineExternal->moduleAudio->LoadBank(bankName);
//				(*it)->loaded_in_heap = true;
//			}
//			return;
//		}
//	}
//	audBankReference = nullptr;
//	LOG("Audio Bank called %s, has not been found during scene loading...", bankName);
//}

std::string& CAudioSource::GetEventName(AudioBank* reference)
{
	reference = nullptr;
	if (audBankReference != nullptr)
		reference = audBankReference;
	return evName;
}

void CAudioSource::SetEventName(std::string& newEventName)
{
	this->evName = newEventName;
}

void CAudioSource::SetBankReference(AudioBank* ref)
{
	this->audBankReference = ref;
}

float CAudioSource::GetVolume()
{
	return this->volume;
}

void CAudioSource::SetVolume(float newVol)
{
	this->volume = MIN(newVol, 99.0f);
	this->volume = MAX(newVol, 0.0f);
	if (!isMuted && evName != "")
		External->audio->ChangeRTPCValue(this->id, std::string("SourceVolume"), this->volume);
}

float CAudioSource::GetPitch()
{
	return this->pitch;
}

void CAudioSource::SetPitch(float newPitch)
{
	this->pitch = MIN(newPitch, 100);
	this->pitch = MAX(newPitch, 0);
	if (!isMuted)
		External->audio->ChangeRTPCValue(this->id, std::string("SourcePitch"), this->pitch);
}

bool CAudioSource::GetPlayOnAwake() const
{
	return this->playOnAwake;
}

void CAudioSource::PlayEvent()
{
	if (this->active)
		External->audio->PlayEvent(this->id, this->evName);
}

void CAudioSource::PauseEvent()
{
	External->audio->PauseEvent(this->id, this->evName);
}

void CAudioSource::ResumeEvent()
{
	External->audio->ResumeEvent(this->id, this->evName);
}

void CAudioSource::StopEvent(std::string eventName)
{
	External->audio->StopEvent(this->id, eventName);
}

void CAudioSource::StopEvent()
{
	External->audio->StopEvent(this->id);
}


unsigned int CAudioSource::GetWwiseID()
{
	return id;
}

void CAudioSource::SetSwitch(std::string groupSwitch, std::string stateSwitch)
{
	External->audio->SetSwitch(this->id, groupSwitch, stateSwitch);
}

bool CAudioSource::IsMuted()
{
	return this->isMuted;
}

void CAudioSource::SetMuted(bool muted)
{
	if (muted)
		External->audio->ChangeRTPCValue(this->id, std::string("SourceVolume"), 0.0f);
	else
	{
		External->audio->ChangeRTPCValue(this->id, std::string("SourceVolume"), this->volume);
		External->audio->ChangeRTPCValue(this->id, std::string("SourcePitch"), this->pitch);
	}
	isMuted = muted;
}

bool CAudioSource::IsMusic()
{
	return isMusic;
}