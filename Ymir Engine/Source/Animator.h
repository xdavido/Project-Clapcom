#pragma once
#include "Globals.h"
#include "Application.h"

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

	float CalculatePreviousTime(ResourceAnimation* lastAnimation, float transitionTime);

	bool CheckBlendMap(ResourceAnimation* animation, std::string animationBlend);

	void CalculateBoneTransform(const AssimpNodeData* node, float4x4 parentTransform);

	std::vector<float4x4> GetFinalBoneMatrices() { return finalBoneMatrices; }

	void SetCurrentAnimation(ResourceAnimation* animation) { currentAnimation = animation; }
	ResourceAnimation* GetCurrentAnimation() { return currentAnimation; }

	void SetPreviousAnimation(ResourceAnimation* animation) { previousAnimation = animation; }
	ResourceAnimation* GetPreviousAnimation() { return previousAnimation; }

	bool FindAnimation(std::string aniationName);

private:
	std::vector<float4x4> finalBoneMatrices;

	float deltaTime; 

	float4x4 identity; 

	// Blending 
	float transitionTime;
	float lastCurrentTime;

public:

	// List of animations
	std::vector<ResourceAnimation> animations;

	ResourceAnimation* currentAnimation = nullptr;
	ResourceAnimation* previousAnimation = nullptr;

};