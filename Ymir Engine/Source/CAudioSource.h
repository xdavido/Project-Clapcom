#pragma once

#ifndef __AUDIOSOURCE_H__
#define __AUDIOSOURCE_H__

#include "Component.h"
#include "AudioBank.h"

class CTransform;

class CAudioSource : public Component
{
public:

	CAudioSource(GameObject* owner);
	virtual ~CAudioSource();

	void OnInspector() override;

	void Update() override;

	//void SaveData(JSON_Object* nObj) override;
	//void LoadData(DEConfig& nObj) override;

	std::string& GetEventName(AudioBank* reference = nullptr);
	void SetEventName(std::string& newEventName);
	void SetBankReference(AudioBank* ref);

	float GetVolume();
	void SetVolume(float newVol);

	float GetPitch();
	void SetPitch(float newPitch);

	bool GetPlayOnAwake() const;

	void PlayEvent();
	void PauseEvent();
	void ResumeEvent();
	void StopEvent(std::string eventName);

	void StopEvent();

	unsigned int GetWwiseID();
	void SetSwitch(std::string groupSwitch, std::string stateSwitch);

	bool IsMuted();
	void SetMuted(bool muted);

	bool IsMusic();

private:

	std::string	evName;
	std::string	audBankName;
	AudioBank* audBankReference;
	float volume;
	float pitch;
	bool playOnAwake;
	bool isMuted;
	unsigned int id;
	bool isMusic;

	CTransform*	gameObjectTransform;

};

#endif // __AUDIOSOURCE_H__