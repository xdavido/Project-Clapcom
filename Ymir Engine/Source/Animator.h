#pragma once
#include "Globals.h"

#include "Animation.h"

class Animation;

struct AssimpNodeData;

struct AnimationController {

	float currentTime;
	Animation* animation;
};

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

	void UpdateCurrentTime() {

	}

	void CalculateBoneTransform(const AssimpNodeData* node, float4x4 parentTransform);

	std::vector<float4x4> GetFinalBoneMatrices() { return finalBoneMatrices; }

private:
	std::vector<float4x4> finalBoneMatrices;

	// Variable to remove
	Animation* currentAnimation; 

	float currentTime; 
	float deltaTime; 

	float4x4 identity; 

	bool backwardsAux;
	bool pingPongAux;
	bool pingPongBackwardsAux;

public:
	// Change to Animation *
	std::vector<Animation> animations;
	std::vector<AnimationController*> animationsPlaying;

	float easeInSpeed;
	float easeOutSpeed;
	float easeInMultiplier;
	float easeOutMultiplier;

};