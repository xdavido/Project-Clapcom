#pragma once

#include "Component.h"

#include "Application.h"

#include "Globals.h"
#include <string>
#include <vector>
#include <map>

#include "Animation.h"
#include "Animator.h"

class GameObject;

class Animator;
class Animation;

class CAnimation : public Component {
public:
	CAnimation(GameObject* owner);
	~CAnimation();

	void Update();

	void OnInspector() override;

	void AddAnimation(Animation newAnimation, std::string animationName);

	void RemoveAnimation(int ID);

private:

public:
	//GameObject* rootBone = nullptr;

	//std::map<std::string, GameObject*> boneMapping;

	bool isPlaying = false;
	bool isLoop = false;
	bool isPingPong = false;

	Animator* animator;

private:

	const int animationMaxNum = 10;
	std::string* animationNames = new std::string[animationMaxNum];
	std::vector<Animation> animations;
	int selectedAnimation = 0;
	bool isSelected = false;

	//sizeof crashing the engine for some reason
	int totalAnimations = 0;
};