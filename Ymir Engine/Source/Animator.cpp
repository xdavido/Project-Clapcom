#include "Animator.h"

#include "External/mmgr/mmgr.h"

Animator::Animator()
{
	finalBoneMatrices.reserve(100);
	for (int i = 0; i < 100; i++) {
		finalBoneMatrices.push_back(identity.identity);
	}
}

Animator::Animator(ResourceAnimation* animation)
{
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
	
	for (int i = 0; i < animations.size(); i++) {
		if (animations[i].isPlaying) {
			UpdateCurrentTime(&animations[i]);
			CalculateBoneTransform(&animations[i].GetRootNode(), identity.identity, animations[i]);
		}
	}

}

void Animator::UpdateCurrentTime(ResourceAnimation* animation) {

	// Backwards
	if (animation->backwards && !animation->pingPong && !animation->easeIn && !animation->easeOut) {
		//Start from the last frame
		if (animation->backwardsAux == true) {
			animation->currentTime = animation->GetDuration();
			animation->backwardsAux = false;
		}
		//If reached first frame, stop or go to last frame
		if (animation->currentTime < 0.0f) {

			if (animation->loop) {
				animation->backwardsAux = true;
			}
			else {
				if (!animation->backwardsAux) {
					StopAnimation(animation);
					ResetAnimation(animation);
					animation->backwardsAux = true;
				}
				animation->backwardsAux = true;
			}
		}
		animation->currentTime -= animation->GetTickPerSecond() * deltaTime * animation->speed;
	}
	else if (!animation->pingPong && !animation->easeIn && !animation->easeOut) {
		animation->currentTime += animation->GetTickPerSecond() * deltaTime * animation->speed;
	}

	//PingPong
	if (animation->pingPong && !animation->easeIn && !animation->easeOut) {
		//PingPong and backwards both on
		if (animation->pingPongBackwardsAux && animation->backwards) {
			animation->currentTime = animation->GetDuration();
			animation->pingPongAux = false;
			animation->pingPongBackwardsAux = false;
		}
		//Going
		if (animation->pingPongAux) {
			animation->currentTime += animation->GetTickPerSecond() * deltaTime * animation->speed;
			if (animation->currentTime > animation->GetDuration()) {
				animation->currentTime = animation->GetDuration();
				animation->pingPongAux = false;
				if (!animation->loop && animation->backwards) {
					animation->pingPongBackwardsAux = true;
					animation->pingPongAux = true;
					StopAnimation(animation);
					ResetAnimation(animation);
				}
			}
		}
		//Returning
		if (!animation->pingPongAux) {
			animation->currentTime -= animation->GetTickPerSecond() * deltaTime * animation->speed;
			if (animation->currentTime < 0.0f) {
				animation->currentTime = 0.0f;
				animation->pingPongAux = true;
				if (!animation->loop && !animation->backwards) {
					StopAnimation(animation);
					ResetAnimation(animation);
				}
			}
		}
	}

	//Ease-In
	if (animation->easeIn) {
		animation->currentTime += animation->GetTickPerSecond() * deltaTime * animation->speed * animation->easeInSpeed;
		animation->easeInSpeed *= animation->easeInMultiplier;
		if (animation->currentTime > animation->GetDuration()) {
			animation->easeInSpeed = 1;
			animation->currentTime = 0.0f;
			if (!animation->loop) {
				StopAnimation(animation);
				ResetAnimation(animation);
			}
		}
	}

	//Ease-Out
	if (animation->easeOut) {
		animation->currentTime += animation->GetTickPerSecond() * deltaTime * animation->speed * animation->easeOutSpeed;
		animation->easeOutSpeed *= animation->easeOutMultiplier;
		if (animation->currentTime > animation->GetDuration()) {
			animation->easeOutSpeed = 1;
			animation->currentTime = 0.0f;
			if (!animation->loop) {
				StopAnimation(animation);
				ResetAnimation(animation);
			}
		}
	}


	// Loop
	if (animation->loop && !animation->backwards && !animation->pingPong && !animation->easeIn && !animation->easeOut) {

		//Loop + Backwards
		if (animation->backwards) {
			animation->currentTime = fmod(animation->GetDuration(), animation->currentTime);
		}
		else {
			animation->currentTime = fmod(animation->currentTime, animation->GetDuration());
		}
	}

	if (animation->currentTime < animation->GetDuration()) {
		//CalculateBoneTransform(&animationsPlaying[i]->animation->GetRootNode(), identity.identity);
	}

	float stepTime = animation->currentTime + animation->GetTickPerSecond() * deltaTime * animation->speed;

	if (stepTime > animation->GetDuration() && !animation->loop && !animation->pingPong) {
		//Leave animation in its final state
		animation->currentTime = animation->GetDuration() - 0.01f;

		animation->currentTime = 0.0f;
		StopAnimation(animation);
		ResetAnimation(animation);
	}
		
		
}

void Animator::PlayAnimation(ResourceAnimation* animation)
{
	animation->isPlaying = true;
	animation->currentTime = 0.0f;
}

void Animator::PauseAnimation(ResourceAnimation* animation) {

	animation->isPlaying = false;
}

void Animator::ResumeAnimation(ResourceAnimation* animation) {

	animation->isPlaying = true;
}

void Animator::StopAnimation(ResourceAnimation* animation)
{
	animation->isPlaying = false;
	animation->currentTime = 0.0f;
	CalculateBoneTransform(&animation->GetRootNode(), identity.identity, *animation);
}

void Animator::ResetAnimation(ResourceAnimation* animation) {
	animation->currentTime = 0.0f;
	animation->backwardsAux = true;
	animation->pingPongAux = true;
	animation->pingPongBackwardsAux = true;
	animation->easeInSpeed = 1;
	animation->easeOutSpeed = 1;
}

void Animator::TransitionTo(ResourceAnimation* lastAnimation, ResourceAnimation* nextAnimation, float transitionTime) {

	float timeToTransition = lastAnimation->GetDuration() * transitionTime;
	float transitionDuration = lastAnimation->GetDuration() - timeToTransition;

	float normalizedTime = (lastAnimation->currentTime - timeToTransition) / (lastAnimation->GetDuration() - timeToTransition);

	if (lastAnimation->currentTime >= timeToTransition) {

		if (!nextAnimation->isPlaying)
			PlayAnimation(nextAnimation);
		
		lastAnimation->intensity = 1 - normalizedTime;
		nextAnimation->intensity = normalizedTime;
	}
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, float4x4 parentTransform, ResourceAnimation &animation)
{
	std::string nodeName = node->name;
	float4x4 nodeTransform = node->transformation; 

	Bone* bone = animation.FindBone(nodeName);
	if (bone) {
		bone->Update(animation.currentTime);
		nodeTransform = bone->GetLocalTransform();
	}

	float4x4 globalTransform = parentTransform * nodeTransform;

	std::map<std::string, BoneInfo> boneInfoMap = animation.GetBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
		int index = boneInfoMap[nodeName].id;
		float4x4 offset = boneInfoMap[nodeName].offset;
		finalBoneMatrices[index] = globalTransform * offset; 
	}

	for (int i = 0; i < node->childrenCount; i++) {
		CalculateBoneTransform(&node->children[i], globalTransform, animation);
	}
}
