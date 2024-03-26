#pragma once
#include "Globals.h"

#include "ResourceAnimation.h"
#include "Model.h"

class ResourceAnimation;
class Model; 

struct AssimpNodeData;

class Animator {
public:
	Animator();
	Animator(ResourceAnimation* animation);
	~Animator();

	void UpdateAnimation(float dt);

	void UpdateCurrentTime(ResourceAnimation* animation);
	
	void PlayAnimation(ResourceAnimation* animation);

	void PauseAnimation();

	void ResumeAnimation();

	void StopAnimation();

	void ResetAnimation(ResourceAnimation* animation);

	void TransitionTo(ResourceAnimation* lastAnimation, ResourceAnimation* nextAnimation, float transitionTime);

	void CalculateBoneTransform(const AssimpNodeData* node, float4x4 parentTransform);

	std::vector<float4x4> GetFinalBoneMatrices() { return finalBoneMatrices; }

	void SetCurrentAnimation(ResourceAnimation* animation) { currentAnimation = animation; }
	ResourceAnimation* GetCurrentAnimation() { return currentAnimation; }

	void SetPreviousAnimation(ResourceAnimation* animation) { previousAnimation = animation; }
	ResourceAnimation* GetPreviousAnimation() { return previousAnimation; }

private:
	std::vector<float4x4> finalBoneMatrices;

	float deltaTime; 

	float4x4 identity; 

	bool blend = true;

public:

	// List of animations
	std::vector<ResourceAnimation> animations;

	ResourceAnimation* currentAnimation = nullptr;
	ResourceAnimation* previousAnimation = nullptr;

};