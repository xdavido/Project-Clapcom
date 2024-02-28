#pragma once

#include "Component.h"

#include "Application.h"

#include "Globals.h"
#include <string>
#include <vector>
#include <map>

#include "Animator.h"

class GameObject;
class CMesh;

class Animator;

class CAnimation : public Component {
public:
	CAnimation(GameObject* owner);
	~CAnimation();

	void Update();

	void OnInspector() override;

private:

public:
	GameObject* rootBone = nullptr;

	std::map<std::string, GameObject*> boneMapping;

	bool isPlaying = false;

	Animator* animator;

private:
	
	

};