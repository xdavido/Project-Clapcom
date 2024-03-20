#pragma once

#include "Component.h"

#include "Application.h"

#include "Globals.h"
#include <string>
#include <vector>
#include <map>

#include "Animator.h"

class GameObject;

class Animator;
class ResourceAnimation; 

class CAnimation : public Component {
public:
	CAnimation(GameObject* owner);
	~CAnimation();

	void Update();

	void OnInspector() override;
	
	// Add animation to animations list
	void AddAnimation(ResourceAnimation &newAnimation);

	void RemoveAnimation(int ID);

	// Plays an animation. ¡Must give name!
	// OverridePrev: Stops all playing animations if true
	void PlayAnimation(std::string animationName = "", bool overridePrev = true);

	// Resumes an animation. If not given a name pauses all playing animations
	void PauseAnimation(std::string animationName = "");

	// Resumes an animation. If not given a name resumes all playing animations
	void ResumeAnimation(std::string animationName = "");

	// Stop an animation. If not given a name stops all animations playing
	void StopAnimation(std::string animationName = "");

	void TransitionTo(std::string animationName = "", float transitionTime = 1.0f);


private:

	void YAnimDragDropTarget();

public:

	Animator* animator;

	int selectedAnimation = -1;

	std::string modelPath;

private:

	bool isSelected = false;
	
};