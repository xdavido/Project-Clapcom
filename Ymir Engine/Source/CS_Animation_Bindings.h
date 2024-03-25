#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "CAnimation.h"
#include "Component.h"


void PlayAnimation(MonoObject*go, MonoString* nameAnim) {

	if (External == nullptr)
		return;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);
	std::string ev = mono_string_to_utf8(nameAnim);

	CAnimation* animComp = dynamic_cast<CAnimation*>(GO->GetComponent(ComponentType::ANIMATION));
	if (animComp != nullptr)
	{
		animComp->PlayAnimation(ev);
		/*audSource->SetEventName(ev);
		audSource->PlayEvent();*/
	}
	else
	{
		LOG("[WARNING] Couldn't play the anim %s. Component was null pointer", ev.c_str());
	}

}

void PauseAnimation(MonoObject* go) {

	if (External == nullptr)
		return;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);
	

	CAnimation* animComp = dynamic_cast<CAnimation*>(GO->GetComponent(ComponentType::ANIMATION));
	if (animComp != nullptr)
	{
		animComp->PauseAnimation();

	}
	else
	{
		LOG("[WARNING] Couldn't find ANIMATION COMPONENT!");
	}
}

void ResumeAnimation(MonoObject* go) {

	if (External == nullptr)
		return;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);


	CAnimation* animComp = dynamic_cast<CAnimation*>(GO->GetComponent(ComponentType::ANIMATION));
	if (animComp != nullptr)
	{
		animComp->ResumeAnimation();

	}
	else
	{
		LOG("[WARNING] Couldn't find ANIMATION COMPONENT!");
	}
}

void StopAnimation(MonoObject* go) {

	if (External == nullptr)
		return;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);


	CAnimation* animComp = dynamic_cast<CAnimation*>(GO->GetComponent(ComponentType::ANIMATION));
	if (animComp != nullptr)
	{
		animComp->StopAnimation();

	}
	else
	{
		LOG("[WARNING] Couldn't find ANIMATION COMPONENT!");
	}
}

void SetLoop(MonoObject* go, MonoString* nameAnim, bool loop) {

	if (External == nullptr)
		return;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);
	std::string ev = mono_string_to_utf8(nameAnim);

	CAnimation* animComp = dynamic_cast<CAnimation*>(GO->GetComponent(ComponentType::ANIMATION));
	if (animComp != nullptr)
	{
		animComp->SetLoop(ev,loop);

	}
	else
	{
		LOG("[WARNING] Couldn't find ANIMATION COMPONENT!");
	}
}

void SetBackward(MonoObject* go, MonoString* nameAnim, bool backwards) {

	if (External == nullptr)
		return;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);
	std::string ev = mono_string_to_utf8(nameAnim);

	CAnimation* animComp = dynamic_cast<CAnimation*>(GO->GetComponent(ComponentType::ANIMATION));
	if (animComp != nullptr)
	{
		animComp->SetBackwards(ev, backwards);

	}
	else
	{
		LOG("[WARNING] Couldn't find ANIMATION COMPONENT!");
	}
}

void SetPingPong(MonoObject* go, MonoString* nameAnim, bool pingPong) {

	if (External == nullptr)
		return;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);
	std::string ev = mono_string_to_utf8(nameAnim);

	CAnimation* animComp = dynamic_cast<CAnimation*>(GO->GetComponent(ComponentType::ANIMATION));
	if (animComp != nullptr)
	{
		animComp->SetPingPong(ev, pingPong);

	}
	else
	{
		LOG("[WARNING] Couldn't find ANIMATION COMPONENT!");
	}
}

void SetSpeed(MonoObject* go, MonoString* nameAnim, float speed) {

	if (External == nullptr)
		return;

	GameObject* GO = External->moduleMono->GameObject_From_CSGO(go);
	std::string ev = mono_string_to_utf8(nameAnim);

	CAnimation* animComp = dynamic_cast<CAnimation*>(GO->GetComponent(ComponentType::ANIMATION));
	if (animComp != nullptr)
	{
		animComp->SetSpeed(ev, speed);		

	}
	else
	{
		LOG("[WARNING] Couldn't find ANIMATION COMPONENT!");
	}
}