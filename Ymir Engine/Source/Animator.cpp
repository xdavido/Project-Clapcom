#include "Animator.h"
#include "Log.h"

Animator::Animator()
{
	currentTime = 0.0; 
	currentAnimation = nullptr;
	finalBoneMatrices.reserve(100);
	for (int i = 0; i < 100; i++) {
		finalBoneMatrices.push_back(identity.identity);
	}
}

Animator::Animator(Animation* animation)
{
	currentTime = 0.0; 
	currentAnimation = animation; 
	finalBoneMatrices.reserve(100);
	
	for (int i = 0; i < 100; i++) {
		finalBoneMatrices.push_back(identity.identity);
	}
}

Animator::~Animator() {

}

void Animator::UpdateAnimation(float dt)
{
	deltaTime = dt; 
	
	if (currentAnimation) {

		currentTime += currentAnimation->GetTickPerSecond() * dt * currentAnimation->speed; 

		if (currentAnimation->loop) {
			currentTime = fmod(currentTime, currentAnimation->GetDuration());
		}

		LOG("cT: %f", currentTime);
		LOG("Duration: %f", currentAnimation->GetDuration());

		if (currentTime < currentAnimation->GetDuration())
			CalculateBoneTransform(&currentAnimation->GetRootNode(), identity.identity);

		float stepTime = currentTime + currentAnimation->GetTickPerSecond() * dt * currentAnimation->speed;

		if (stepTime > currentAnimation->GetDuration() && !currentAnimation->loop) {
			//Leave animation in its final state
			currentTime = currentAnimation->GetDuration() - 0.01f;
			CalculateBoneTransform(&currentAnimation->GetRootNode(), identity.identity);

			StopAnimation();
		}
	}
}

void Animator::PlayAnimation(Animation* animation)
{
	currentAnimation = animation; 
	currentTime = 0.0f; 
}

void Animator::StopAnimation()
{
	currentAnimation->isPlaying = false;;
	currentTime = 0.0f;
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, float4x4 parentTransform)
{
	std::string nodeName = node->name;
	float4x4 nodeTransform = node->transformation; 

	Bone* bone = currentAnimation->FindBone(nodeName);
	if (bone) {
		bone->Update(currentTime);
		nodeTransform = bone->GetLocalTransform();
	}

	float4x4 globalTransform = parentTransform * nodeTransform;

	std::map<std::string, BoneInfo> boneInfoMap = currentAnimation->GetBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
		int index = boneInfoMap[nodeName].id;
		float4x4 offset = boneInfoMap[nodeName].offset;
		finalBoneMatrices[index] = globalTransform * offset; 
	}

	for (int i = 0; i < node->childrenCount; i++) {
		CalculateBoneTransform(&node->children[i], globalTransform);
	}
}
