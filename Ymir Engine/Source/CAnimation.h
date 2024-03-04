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

	bool isPlaying = false;
	bool isLoop = false;
	bool isPingPong = false;

	Animator* animator;

private:

	std::vector<Animation> animations;
	int selectedAnimation = -1;
	bool isSelected = false;
	int selectedAnimationPlaying = -1;
	std::string aniName;

	//sizeof crashing the engine for some reason so using int for now
	int totalAnimations = -1;
};