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

	void AddAnimation(Animation &newAnimation, std::string animationName);

	void RemoveAnimation(int ID);

private:

public:

	Animator* animator;

	int selectedAnimation = -1;

	int totalAnimations = -1;

	std::string modelPath;

private:

	
	bool isSelected = false;
	int selectedAnimationPlaying = -1;

	//sizeof crashing the engine for some reason so using int for now
	
};