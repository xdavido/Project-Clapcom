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

	// Plays an animation. !!Must give name!!
	void PlayAnimation(std::string animationName);

	// Resumes current playing animation.
	void PauseAnimation();

	// Resumes current playing animation.
	void ResumeAnimation();

	// Stop current playing animation.
	void StopAnimation();

	// Set animation to loop. If not given a name it applies to all animations
	void SetLoop(std::string animationName = "", bool loop = true);

	// Set animation to go backwards. If not given a name it applies to all animations
	void SetBackwards(std::string animationName = "", bool backwards = true);

	// Set animation to pingpong. If not given a name it applies to all animations
	void SetPingPong(std::string animationName = "", bool pingPong = true);

	// Set animation speed. If not given a name it applies to all animations
	void SetSpeed(std::string animationName = "", float speed = true);

	// Specify which animations will blend into and the duration of the blend. If not given a name it applies to all animations
	void AddBlendOption(std::string animationName = "", std::string blendName = "", float frames = 10.0f);

	// Set animation to reset to frame 0 when ending. If not given a name it applies to all animations
	void SetResetToZero(std::string animationName = "", bool resetToZero = true);

	bool HasFinished(std::string animationName);





private:

	void YAnimDragDropTarget();

public:

	Animator* animator;

	int selectedAnimation = -1;

	std::string modelPath;

private:

	bool isSelected = false;
	
};