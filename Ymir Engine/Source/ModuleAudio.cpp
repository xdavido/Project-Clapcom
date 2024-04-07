#include "ModuleAudio.h"
#include "Wwise.h"
#include "External/Wwise/low_level_IO/Win32/AkFilePackageLowLevelIOBlocking.h"
#include "CAudioListener.h"
#include "CAudioSource.h"
#include "External/MathGeoLib/include/MathGeoLib.h"
#include "JsonFile.h"

#include "External/Wwise/AK/Plugin/AkVorbisDecoderFactory.h"
#pragma comment(lib, "Source/External/Wwise/Release(StaticCRT)/lib/AkVorbisDecoder.lib")

#include <assert.h>
#include "ModuleAudio.h"
#include "Log.h"

#include "External/mmgr/mmgr.h"

CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

ModuleAudio::ModuleAudio(Application* app, bool start_enabled) : Module(app, start_enabled), wwiseListenerHasToUpdate(false), defaultListener(nullptr), masterVolume(50.0f), musicVolume(50.0f), fxVolume(50.0f), uiBankRef(nullptr)
{
#ifdef STANDALONE
	firstFrame = true;
#endif STANDALONE
}

ModuleAudio::~ModuleAudio()
{

}

bool ModuleAudio::Init()
{
	//Init Memory Manager
	AkMemSettings memSettings;

	AK::MemoryMgr::GetDefaultSettings(memSettings);

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		LOG("[ERROR] Could not create the memory manager.");
		return false;
	}

	//Init Stream Manager
	AkStreamMgrSettings stmSettings;

	AK::StreamMgr::GetDefaultSettings(stmSettings);

	// Customize the Stream Manager settings here.

	if (!AK::StreamMgr::Create(stmSettings))
	{
		LOG("[ERROR] Could not create the Streaming Manager.");
		return false;
	}

	// Create a streaming device with blocking low-level I/O handshaking.
	AkDeviceSettings deviceSettings;

	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		LOG("[ERROR] Could not create the streaming device and Low-Level I/O system.");
		return false;
	}

	//Init Sound Engine
	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;

	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		LOG("[ERROR] Could not initialize the Sound Engine.");
		return false;
	}

	//Init Music Engine
	AkMusicSettings musicInit;

	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		LOG("[ERROR] Could not initialize the Music Engine.");
		return false;
	}

	// Init Spatial Audio

	AkSpatialAudioInitSettings settings; // The constructor fills AkSpatialAudioInitSettings with the recommended default settings. 

	if (AK::SpatialAudio::Init(settings) != AK_Success)
	{

		LOG("[ERROR] Could not initialize the Spatial Audio.");

		return false;

	}

	g_lowLevelIO.SetBasePath(AKTEXT("Assets/SoundBanks/"));

	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

	AkBankID bankID;
	AKRESULT ret = AK::SoundEngine::LoadBank("Init.bnk", bankID);
	if (ret != AK_Success)
	{
		LOG("[ERROR] Could not initialize Init.bnk.");
		return false;
	}

	return true;
}

bool ModuleAudio::Start()
{
	if (!LoadBanksInfo())
	{
		LOG("[ERROR] Audio Manager couldn't load data from SoundbanksInfo.json");
	}
	return true;
}

update_status ModuleAudio::Update(float dt)
{
#ifdef STANDALONE
	if (firstFrame)
	{
		PlayOnAwake();
		firstFrame = false;
	}
#endif
	if (wwiseListenerHasToUpdate)
	{
		UpdateWwiseListener();
		wwiseListenerHasToUpdate = false;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleAudio::PostUpdate(float dt)
{
	if (defaultListener != nullptr && defaultListener->active)
	{
		//TODO when there is no sound listener or is deactivated all sound must be muted (but still rendering!)
	}

	// Process bank requests, events, positions, RTPC, etc.
	AK::SoundEngine::RenderAudio();

	return UPDATE_CONTINUE;
}

bool ModuleAudio::CleanUp()
{
	AK::SoundEngine::UnregisterAllGameObj();
	AK::SoundEngine::ClearBanks();

	//Spatial audio doesn't have a Term() method

	// Terminate the music engine
	AK::MusicEngine::Term();

	// Terminate the sound engine
	AK::SoundEngine::Term();

	// that lives in the Stream Manager, and unregisters itself as the File Location Resolver.
	g_lowLevelIO.Term();

	if (AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();

	// Terminate the Memory Manager
	AK::MemoryMgr::Term();

	audio_sources.clear();

	std::vector<AudioBank*>::iterator it;
	for (it = banks.begin(); it != banks.end(); ++it)
	{
		(*it)->events.clear();
		(*it)->actions.clear();
		delete (*it);
		(*it) = nullptr;
	}
	banks.clear();
	musicSource.clear();

	defaultListener = nullptr;

	return true;
}

void ModuleAudio::RegisterNewAudioObject(unsigned int id)
{
	AK::SoundEngine::RegisterGameObj(id);
}

void ModuleAudio::UnRegisterAudioObject(unsigned int id)
{
	AK::SoundEngine::UnregisterGameObj(id);
}

void ModuleAudio::StopAllSounds() const
{
	AK::SoundEngine::StopAll();
}

void ModuleAudio::PauseAllSounds() const
{
	std::vector<CAudioSource*>::const_iterator it;
	for (it = audio_sources.begin(); it != audio_sources.end(); ++it)
	{
#ifdef STANDALONE
		if (!(*it)->IsMusic())
			(*it)->PauseEvent();
#else
		(*it)->PauseEvent();
#endif // STANDALONE

	}
}

void ModuleAudio::ResumeAllSounds() const
{
	std::vector<CAudioSource*>::const_iterator it;
	for (it = audio_sources.begin(); it != audio_sources.end(); ++it)
	{
#ifdef STANDALONE
		if (!(*it)->IsMusic())
			(*it)->ResumeEvent();
#else
		(*it)->ResumeEvent();
#endif
	}
}

void ModuleAudio::PlayOnAwake()
{
	std::vector<CAudioSource*>::const_iterator it;

	for (it = audio_sources.begin(); it != audio_sources.end(); ++it)
	{
		if ((*it)->GetPlayOnAwake())
			(*it)->PlayEvent();
	}
}

void ModuleAudio::PlayEvent(unsigned int id, std::string& eventName)
{
	AK::SoundEngine::PostEvent(eventName.c_str(), id);
	bool isMusic = false;
	for (int i = 0; i < musicSource.size(); ++i) {
		if (id == musicSource[i]->GetWwiseID()) {
			isMusic = true;
			break;
		}
	}
	if (isMusic)
	{
		ChangeRTPCValue(id, std::string("SourceVolume"), musicVolume);
		return;
	}
	else
	{
		ChangeRTPCValue(id, std::string("SourceVolume"), fxVolume);
		return;
	}
}

void ModuleAudio::StopEvent(unsigned int id, std::string& eventName) const
{
	AK::SoundEngine::ExecuteActionOnEvent(eventName.c_str(), AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Stop, id);
}

void ModuleAudio::StopEvent(unsigned int id) const
{
	AK::SoundEngine::StopAll(id);
}


void ModuleAudio::PauseEvent(unsigned int id, std::string& eventName) const
{
	AK::SoundEngine::ExecuteActionOnEvent(eventName.c_str(), AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Pause, id);
}

void ModuleAudio::ResumeEvent(unsigned int id, std::string& eventName) const
{
	AK::SoundEngine::ExecuteActionOnEvent(eventName.c_str(), AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Resume, id);
}

void ModuleAudio::SetState(std::string& groupState, std::string& state)
{
	AK::SoundEngine::SetState(groupState.c_str(), state.c_str());
}

void ModuleAudio::SetSwitch(unsigned int id, std::string& groupSwitch, std::string& stateSwitch)
{
	AK::SoundEngine::SetSwitch(groupSwitch.c_str(), stateSwitch.c_str(), id);
}


void ModuleAudio::StopComponent(unsigned int id) const
{
	AK::SoundEngine::PostEvent("Stop_Object", id);
	//AK::SoundEngine::ExecuteActionOnPlayingID(AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Stop, id); //this doesn't work
}

void ModuleAudio::PauseComponent(unsigned int id) const
{
	AK::SoundEngine::PostEvent("Pause_Object", id); //Note that this is not a master resume, that means that will only remove 1 pause action if many
	//AK::SoundEngine::ExecuteActionOnPlayingID(AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Pause, id); //this doesn't work
}

void ModuleAudio::ResumeComponent(unsigned int id) const
{
	AK::SoundEngine::PostEvent("Resume_Object", id);
	//AK::SoundEngine::ExecuteActionOnPlayingID(AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Resume, id); //this doesn't work
}

void ModuleAudio::ChangeRTPCValue(unsigned int id, std::string& RTPCname, float value)
{
	AK::SoundEngine::SetRTPCValue(RTPCname.c_str(), value, id);
}



bool ModuleAudio::LoadBanksInfo()
{
	JSON_Value* banksInfo = json_parse_file("Assets/SoundBanks/SoundbanksInfo.json");

	if (banksInfo == NULL)
		return false;

	JSON_Object* banksObject(json_value_get_object(banksInfo));
	JSON_Object* banksData(json_object_get_object(banksObject, "SoundBanksInfo"));
	JSON_Array* banksArray = json_object_get_array(banksData, "SoundBanks");

	for (unsigned int cursor = 0; cursor < json_array_get_count(banksArray); ++cursor)
	{
		JSON_Object* tmp(json_array_get_object(banksArray, cursor));
		if (strcmp(json_object_get_string(tmp, "ShortName"), "Init") != 0)
		{
			AudioBank* tmpBank = new AudioBank;
			JSON_Array* tmpEvents;
			tmpBank->loaded_in_heap = false;
			tmpBank->bank_name = json_object_get_string(tmp, "ShortName");

			tmpEvents = json_object_get_array(tmp, "IncludedEvents");

			// ec stands for event cursor
			for (unsigned int ec = 0; ec < json_array_get_count(tmpEvents); ++ec)
			{
				JSON_Object* aux(json_array_get_object(tmpEvents, ec));
				if (!((std::string)json_object_get_string(aux, "Name")).find("Play_"))
				{
					tmpBank->events[std::stoull(json_object_get_string(aux, "Id"))] = json_object_get_string(aux, "Name");
				}
				else
				{
					tmpBank->actions[std::stoull(json_object_get_string(aux, "Id"))] = json_object_get_string(aux, "Name");
				}
			}
			banks.push_back(tmpBank);
			if (strcmp(json_object_get_string(tmp, "ShortName"), "UI") == 0)
			{
				LoadBank(std::string("UI.bnk"));
				uiBankRef = banks.at(banks.size() - 1);
			}
		}
	}

	return true;
}

bool ModuleAudio::LoadBank(std::string& name)
{
	AkBankID id;
	AKRESULT eResult;
	//std::string extension = "Game/Assets/SoundBanks" + name;
	eResult = AK::SoundEngine::LoadBank(name.c_str(), id);
	if (eResult == AK_Success)
	{
		LOG("Bank' '%s' has been loaded successfully", name.c_str());
		return true;
	}
	else
	{
		LOG("[ERROR] Error loading '%s'", name.c_str());
		return false;
	}

}

bool ModuleAudio::UnLoadBank(std::string& name)
{
	AKRESULT res = AK::SoundEngine::UnloadBank(name.c_str(), NULL);
	if (res != AK_Success)
	{
		LOG("%s bank couldn't be unloaded", name);
		return false;
	}
	return true;
}

void ModuleAudio::UnLoadAllBanks()
{
	std::vector<AudioBank*>::iterator it;
	for (it = banks.begin(); it != banks.end(); ++it)
	{
		if ((*it)->loaded_in_heap)
			(*it)->loaded_in_heap = !UnLoadBank((*it)->bank_name);
	}
}

void ModuleAudio::WwiseListnerHasToUpdate()
{
	wwiseListenerHasToUpdate = true;
}

void ModuleAudio::AddAudioSource(CAudioSource* new_source)
{
	audio_sources.push_back(new_source);
}

void ModuleAudio::RemoveAudioSource(CAudioSource* source)
{
	std::vector<CAudioSource*>::const_iterator it;
	for (it = audio_sources.begin(); it != audio_sources.end(); ++it)
	{
		if ((*it) == source)
		{
			audio_sources.erase(it);
			return;
		}
	}
}

void ModuleAudio::SetAudioObjTransform(unsigned int id, float3& pos, float3& forward, float3& up)
{
	AkSoundPosition newPos;
	newPos.SetPosition(-pos.x, -pos.y, -pos.z);
	newPos.SetOrientation(forward.x, forward.y, forward.z, up.x, up.y, up.z);

	AK::SoundEngine::SetPosition(id, newPos);
}

void ModuleAudio::SetBusVolume(float volume)
{
	if (defaultListener != nullptr)
		ChangeRTPCValue(defaultListener->GetID(), std::string("BusVolume"), volume);

	masterVolume = volume;
}

void ModuleAudio::SetMusicVolume(float volume)
{
	for (int i = 0; i < musicSource.size(); ++i) {
		musicSource[i]->SetVolume(volume);
	}
	musicVolume = volume;
}

void ModuleAudio::SetSFXVolume(float volume)
{
	fxVolume = volume;
}

//this updates the listener that Wwise uses to be the Module Audio default listener
void ModuleAudio::UpdateWwiseListener()
{
	AkGameObjectID id = 0;

	if (defaultListener != nullptr)
	{
		id = defaultListener->GetID();
	}
	else
	{
		LOG("[WARNING] There is no audio listener component active. Sounds won't be heard");
	}

	// Set one listener as the default.
	AK::SoundEngine::SetDefaultListeners(&id, 1);
}


