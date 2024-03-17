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
	
	// Add animation to animations list
	void AddAnimation(Animation &newAnimation);

	void RemoveAnimation(int ID);

	void PlayAnimation(std::string animationName = "");

	void ResumeAnimation(std::string animationName = "");

	void StopAnimation(std::string animationName = "");

private:

public:

	Animator* animator;

	int selectedAnimation = -1;

	std::string modelPath;

private:

	bool isSelected = false;
	
};