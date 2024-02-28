#pragma once

#ifndef __MODULEAUDIO_H__
#define __MODULEAUDIO_H__

#include <string>
#include <vector>

#include "External/Wwise/AK/SoundEngine/Common/AkTypes.h"
#include "External/MathGeoLib/include/MathGeoLibFwd.h"

#include "Module.h"
#include "Globals.h"
#include "AudioBank.h"

class CAudioListener;
class CAudioSource;

class ModuleAudio : public Module
{
public:

	// Construct / Destruct Methods
	ModuleAudio(Application* app, bool start_enabled = true);
	virtual ~ModuleAudio();

	// Application
	bool Init();
	bool Start();

	update_status Update(float dt);
	update_status PostUpdate(float dt);

	bool CleanUp();

	// Audio Management
	void RegisterNewAudioObject(unsigned int id);
	void UnRegisterAudioObject(unsigned int id);

	void StopAllSounds() const;
	void PauseAllSounds() const;
	void ResumeAllSounds() const;

	void PlayOnAwake();

	void PlayEvent(unsigned int id, std::string& eventName);
	void StopEvent(unsigned int id, std::string& eventName) const;
	void StopEvent(unsigned int id) const;
	void PauseEvent(unsigned int id, std::string& eventName) const;
	void ResumeEvent(unsigned int id, std::string& eventName) const;

	void SetState(std::string& groupState, std::string& state);
	void SetSwitch(unsigned int id, std::string& groupSwitch, std::string& stateSwitch);

	void StopComponent(unsigned int id) const;
	void PauseComponent(unsigned int id) const;
	void ResumeComponent(unsigned int id) const;

	void ChangeRTPCValue(unsigned int id, std::string& RTPCname, float value);

	bool LoadBanksInfo();
	bool LoadBank(std::string& name);
	bool UnLoadBank(std::string& name);

	void UnLoadAllBanks();

	void WwiseListnerHasToUpdate();

	void AddAudioSource(CAudioSource* new_source);
	void RemoveAudioSource(CAudioSource* source);

	void SetAudioObjTransform(unsigned int id, float3& pos, float3& forward, float3& up);

	void SetBusVolume(float volume);
	void SetMusicVolume(float volume);
	void SetSFXVolume(float volume);

private:

	void UpdateWwiseListener();

public:

	std::vector<AudioBank*> banks;
	std::vector<CAudioSource*> audio_sources;
	CAudioListener* defaultListener;
	std::vector<CAudioSource*> musicSource;

	float masterVolume;
	float musicVolume;
	float fxVolume;

	AudioBank* uiBankRef;

private:

	bool wwiseListenerHasToUpdate;
	bool firstFrame;

};

#endif // __MODULEAUDIO_H__