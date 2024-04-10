#pragma once

#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "CAudioSource.h"
#include "Component.h"
#include "ModuleAudio.h"

void PlayAudio(MonoObject* go, MonoString* eventAudio)
{
	if (External == nullptr)
		return;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);
	std::string ev = mono_string_to_utf8(eventAudio);

	CAudioSource* audSource = dynamic_cast<CAudioSource*>(GO->GetComponent(ComponentType::AUDIO_SOURCE));
	if (audSource != nullptr)
	{
		External->audio->PlayEvent(audSource->id, ev);
		/*audSource->SetEventName(ev);
		audSource->PlayEvent();*/
	}
	else
	{
		LOG("[WARNING] Couldn't play the audio %s. Component was null pointer", ev.c_str());
	}
}

void ResumeAudio(MonoObject* go)
{
	if (External == nullptr)
		return;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);

	CAudioSource* audSource = dynamic_cast<CAudioSource*>(GO->GetComponent(ComponentType::AUDIO_SOURCE));
	if (audSource != nullptr)
	{
		audSource->ResumeEvent();
	}
	else
	{
		LOG("[WARNING] Couldn't resume the audio. Component was null pointer");
	}
}

void PauseAudio(MonoObject* go)
{
	if (External == nullptr)
		return;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);

	CAudioSource* audSource = dynamic_cast<CAudioSource*>(GO->GetComponent(ComponentType::AUDIO_SOURCE));
	if (audSource != nullptr)
	{
		audSource->PauseEvent();
	}
	else
	{
		LOG("[WARNING] Couldn't pause the audio. Component was null pointer");
	}
}


void StopOneAudio(MonoObject* go, MonoString* eventAudio)
{
	if (External == nullptr)
		return;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);
	CAudioSource* audSource = dynamic_cast<CAudioSource*>(GO->GetComponent(ComponentType::AUDIO_SOURCE));
	if (audSource != nullptr && eventAudio != nullptr)
	{
		std::string ev = mono_string_to_utf8(eventAudio);
		audSource->StopEvent(ev);
	}
	else
	{
		LOG("[WARNING] Couldn't stop the audio. Component was null pointer");
	}
}

void StopAudio(MonoObject* go)
{
	if (External == nullptr)
		return;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);
	CAudioSource* audSource = dynamic_cast<CAudioSource*>(GO->GetComponent(ComponentType::AUDIO_SOURCE));
	if (audSource != nullptr)
	{
		audSource->StopEvent();
	}
	else
	{
		LOG("[WARNING] Couldn't stop the audio. Component was null pointer");
	}
}


void SetState(MonoString* stateGroupString, MonoString* stateString)
{
	if (External == nullptr)
		return;
	std::string stateGroup = mono_string_to_utf8(stateGroupString);
	std::string state = mono_string_to_utf8(stateString);
	External->audio->SetState(stateGroup, state);
}

void SetSwitch(MonoObject* go, MonoString* switchGroupString, MonoString* stateSwitchString)
{
	if (External == nullptr)
		return;
	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);

	if (GO == nullptr)
		return;

	CAudioSource* audSource = dynamic_cast<CAudioSource*>(GO->GetComponent(ComponentType::AUDIO_SOURCE));

	if (audSource == nullptr)
		return;

	std::string stateGroupSwitch = mono_string_to_utf8(switchGroupString);
	std::string stateSwitch = mono_string_to_utf8(stateSwitchString);

	audSource->SetSwitch(stateGroupSwitch, stateSwitch);

}

float GetVolume(MonoObject* go)
{
	if (External == nullptr)
		return NULL;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);

	CAudioSource* audSource = dynamic_cast<CAudioSource*>(GO->GetComponent(ComponentType::AUDIO_SOURCE));
	if (audSource != nullptr)
	{
		return audSource->GetVolume();
	}
	else
	{
		LOG("[WARNING] Couldn't get the volume. Component was null pointer");
		return NULL;
	}
}

void SetVolume(MonoObject* go, float volume)
{
	if (External == nullptr)
		return;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);

	CAudioSource* audSource = dynamic_cast<CAudioSource*>(GO->GetComponent(ComponentType::AUDIO_SOURCE));
	if (audSource != nullptr)
	{
		audSource->SetVolume(volume);
	}
	else
	{
		LOG("[WARNING] Couldn't set the volume to %.2f. Component was null pointer", volume);
	}
}

float GetPitch(MonoObject* go)
{
	if (External == nullptr)
		return NULL;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);

	CAudioSource* audSource = dynamic_cast<CAudioSource*>(GO->GetComponent(ComponentType::AUDIO_SOURCE));
	if (audSource != nullptr)
	{
		return audSource->GetPitch();
	}
	else
	{
		LOG("[WARNING] Couldn't get the pitch. Component was null pointer");
		return NULL;
	}
}

void SetPitch(MonoObject* go, float pitch)
{
	if (External == nullptr)
		return;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);

	CAudioSource* audSource = dynamic_cast<CAudioSource*>(GO->GetComponent(ComponentType::AUDIO_SOURCE));
	if (audSource != nullptr)
	{
		audSource->SetPitch(pitch);
	}
	else
	{
		LOG("[WARNING] Couldn't set the pitch to %.2f. Component was null pointer", pitch);
	}
}

bool GetMuted(MonoObject* go)
{
	if (External == nullptr)
		return NULL;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);

	CAudioSource* audSource = dynamic_cast<CAudioSource*>(GO->GetComponent(ComponentType::AUDIO_SOURCE));
	if (audSource != nullptr)
	{
		return audSource->IsMuted();
	}
	else
	{
		LOG("[WARNING] Couldn't get the mute variable. Component was null pointer");
	}
}

void SetMuted(MonoObject* go, bool muted)
{
	if (External == nullptr)
		return;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);

	CAudioSource* audSource = dynamic_cast<CAudioSource*>(GO->GetComponent(ComponentType::AUDIO_SOURCE));
	if (audSource != nullptr)
	{
		audSource->SetMuted(muted);
	}
	else
	{
		LOG("[WARNING] Couldn't set the mute to the given value. Component was null pointer");
	}
}

void PauseAllAudios()
{
	if (External == nullptr)
		return;

	External->audio->PauseAllSounds();
}

void ResumeAllAudios()
{
	if (External == nullptr)
		return;

	External->audio->ResumeAllSounds();
}

void StopAllAudios()
{
	if (External == nullptr)
		return;

	External->audio->StopAllSounds();
}