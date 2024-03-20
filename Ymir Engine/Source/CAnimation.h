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

	// Plays an animation. �Must give name!
	// OverridePrev: Stops all playing animations if true
	void PlayAnimation(std::string animationName = "", bool overridePrev = true);

	// Resumes an animation. If not given a name pauses all playing animations
	void PauseAnimation(std::string animationName = "");

	// Resumes an animation. If not given a name resumes all playing animations
	void ResumeAnimation(std::string animationName = "");

	// Stop an animation. If not given a name stops all animations playing
	void StopAnimation(std::string animationName = "");

	void TransitionTo(std::string animationName = "", float transitionTime = 1.0f);

	// Set animation to loop. If not given a name it applies to all animations
	void SetLoop(std::string animationName = "", bool loop = true);

	// Set animation to go backwards. If not given a name it applies to all animations
	void SetBackwards(std::string animationName = "", bool backwards = true);

	// Set animation to pingpong. If not given a name it applies to all animations
	void SetPingPong(std::string animationName = "", bool pingPong = true);

	// Set animation speed. If not given a name it applies to all animations
	void SetSpeed(std::string animationName = "", float loop = true);


private:

	void YAnimDragDropTarget();

public:

	Animator* animator;

	int selectedAnimation = -1;

	std::string modelPath;

private:

	bool isSelected = false;
	
};