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
	
	Animation* GetCurrentAnimation() { return currentAnimation; }
	float GetCurrentAnimationTime() { return currentTime; }
	void SetCurrentAnimationTime(float ct) { currentTime = ct; }

	void UpdateAnimation(float dt);

	void PlayAnimation(Animation* animation);

	void PauseAnimation();

	void ResumeAnimation();

	void StopAnimation();

	void CalculateBoneTransform(const AssimpNodeData* node, float4x4 parentTransform);

	std::vector<float4x4> GetFinalBoneMatrices() { return finalBoneMatrices; }

private:
	std::vector<float4x4> finalBoneMatrices;
	Animation* currentAnimation; 
	float currentTime; 
	float deltaTime; 

	float4x4 identity; 

	bool backwardsAux;
	bool pingPongAux;
	bool pingPongBackwardsAux;

public:
	std::vector<Animation> animations;
	float easeInSpeed;
	float easeOutSpeed;
	float easeInMultiplier;
	float easeOutMultiplier;

};