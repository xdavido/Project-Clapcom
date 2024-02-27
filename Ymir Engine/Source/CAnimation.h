#pragma once

#include "Component.h"

#include "Application.h"

#include "Globals.h"
#include "Model.h"
#include <string>
#include <vector>
#include <map>

class GameObject;
class CMesh;
class Animation;
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

private:
	//Doesn't compile
	//Animator ourAnimator;

};