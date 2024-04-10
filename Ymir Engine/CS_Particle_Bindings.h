#pragma once
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"
#include "CAnimation.h"
#include "Component.h"


void PlayEmitter(MonoObject* go, MonoString* nameAnim) {

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

//TODO TONY / TODO TONI: Esto me esta llorando de todo mal, no se que le pasa asi que te lo dejo