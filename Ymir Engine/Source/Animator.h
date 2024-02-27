#pragma once
#include "Globals.h"
#include "Animation.h"

class Animator {
public:
	Animator(Animation* animation);
	~Animator();
	
	void UpdateAnimation(float dt);

	void PlayAnimation(Animation* animation);

	void CalculateBoneTransform(const AssimpNodeData* node, float4x4 parentTransform);

	std::vector<float4x4> GetFinalBoneMatrices() { return finalBoneMatrices; }

private:
	std::vector<float4x4> finalBoneMatrices;
	Animation* currentAnimation; 
	float currentTime; 
	float deltaTime; 

	float4x4 identity; 
};