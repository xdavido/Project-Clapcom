#pragma once
#include "Globals.h"

#include "Animation.h"

class Animation;

struct AssimpNodeData;

class Animator {
public:
	Animator();
	Animator(Animation* animation);
	~Animator();

	void UpdateAnimation(float dt);

	void UpdateCurrentTime(Animation* animation);
	
	void PlayAnimation(Animation* animation);

	void PauseAnimation(Animation* animation);

	void ResumeAnimation(Animation* animation);

	void StopAnimation(Animation* animation);

	void ResetAnimation(Animation* animation);

	void CalculateBoneTransform(const AssimpNodeData* node, float4x4 parentTransform, Animation &animation);

	std::vector<float4x4> GetFinalBoneMatrices() { return finalBoneMatrices; }

private:
	std::vector<float4x4> finalBoneMatrices;

	float deltaTime; 

	float4x4 identity; 

public:
	// Change to Animation *
	std::vector<Animation> animations;

};