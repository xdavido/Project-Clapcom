#pragma once
#include "Globals.h"

#include "ResourceAnimation.h"

class Animation;
class ResourceAnimation;

struct AssimpNodeData;

class Animator {
public:
	Animator();
	Animator(ResourceAnimation* animation);
	~Animator();

	void UpdateAnimation(float dt);

	void UpdateCurrentTime(ResourceAnimation* animation);
	
	void PlayAnimation(ResourceAnimation* animation);

	void PauseAnimation(ResourceAnimation* animation);

	void ResumeAnimation(ResourceAnimation* animation);

	void StopAnimation(ResourceAnimation* animation);

	void ResetAnimation(ResourceAnimation* animation);

	void TransitionTo(ResourceAnimation* lastAnimation, ResourceAnimation* nextAnimation, float transitionTime);

	void CalculateBoneTransform(const AssimpNodeData* node, float4x4 parentTransform, ResourceAnimation&animation);

	std::vector<float4x4> GetFinalBoneMatrices() { return finalBoneMatrices; }

private:
	std::vector<float4x4> finalBoneMatrices;

	float deltaTime; 

	float4x4 identity; 

public:

	std::vector<ResourceAnimation> animations;

};