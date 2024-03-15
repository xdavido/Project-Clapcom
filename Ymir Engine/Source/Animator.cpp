#include "Animator.h"

#include "External/mmgr/mmgr.h"

Animator::Animator()
{
	currentAnimation = nullptr;

	backwardsAux = true;
	pingPongAux = true;
	pingPongBackwardsAux = true;
	currentTime = 0.0; 
	currentAnimation = nullptr;
	easeInSpeed = 1;
	easeOutSpeed = 1;
	easeInMultiplier = 1.025f;
	easeOutMultiplier = 0.995f;
	finalBoneMatrices.reserve(100);
	for (int i = 0; i < 100; i++) {
		finalBoneMatrices.push_back(identity.identity);
	}
}

Animator::Animator(Animation* animation)
{
	currentAnimation = nullptr;

	backwardsAux = true;
	pingPongAux = true;
	pingPongBackwardsAux = true;
	easeInSpeed = 1;
	easeOutSpeed = 1;
	easeInMultiplier = 1.025f;
	easeOutMultiplier = 0.995f;
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

		// Backwards
		if (currentAnimation->backwards && !currentAnimation->pingPong && !currentAnimation->easeIn && !currentAnimation->easeOut) {
			//Start from the last frame
			if (backwardsAux == true) {
				currentTime = currentAnimation->GetDuration();
				backwardsAux = false;
			}
			//If reached first frame, stop or go to last frame
			if (currentTime < 0.0f) {

				if (currentAnimation->loop) {
					backwardsAux = true;
				}
				else {
					if (!backwardsAux) {
						StopAnimation();
						backwardsAux = true;
					}
					backwardsAux = true;
				}
			}
			currentTime -= currentAnimation->GetTickPerSecond() * dt * currentAnimation->speed;
		}
		else if (!currentAnimation->pingPong && !currentAnimation->easeIn && !currentAnimation->easeOut) {
			currentTime += currentAnimation->GetTickPerSecond() * dt * currentAnimation->speed;
		}

		//PingPong
		if (currentAnimation->pingPong && !currentAnimation->easeIn && !currentAnimation->easeOut) {
			//PingPong and backwards both on
			if (pingPongBackwardsAux && currentAnimation->backwards) {
				currentTime = currentAnimation->GetDuration();
				pingPongAux = false;
				pingPongBackwardsAux = false;
			}
			//Going
			if (pingPongAux) {
				currentTime += currentAnimation->GetTickPerSecond() * dt * currentAnimation->speed;
				if (currentTime > currentAnimation->GetDuration()) {
					currentTime = currentAnimation->GetDuration();
					pingPongAux = false;
					if (!currentAnimation->loop && currentAnimation->backwards) {
						pingPongBackwardsAux = true;
						pingPongAux = true;
						StopAnimation();
					}
				}
			}
			//Returning
			if (!pingPongAux) {
				currentTime -= currentAnimation->GetTickPerSecond() * dt * currentAnimation->speed;
				if (currentTime < 0.0f) {
					currentTime = 0.0f;
					pingPongAux = true;
					if (!currentAnimation->loop && !currentAnimation->backwards) {
						StopAnimation();
					}
				}
			}
		}

		//Ease-In
		if (currentAnimation->easeIn) {
			currentTime += currentAnimation->GetTickPerSecond() * dt * currentAnimation->speed * easeInSpeed;
			easeInSpeed *= easeInMultiplier;
			if (currentTime > currentAnimation->GetDuration()) {
				easeInSpeed = 1;
				currentTime = 0.0f;
				if (!currentAnimation->loop) {
					StopAnimation();
				}
			}
		}

		//Ease-Out
		if (currentAnimation->easeOut) {
			currentTime += currentAnimation->GetTickPerSecond() * dt * currentAnimation->speed * easeOutSpeed;
			easeOutSpeed *= easeOutMultiplier;
			if (currentTime > currentAnimation->GetDuration()) {
				easeOutSpeed = 1;
				currentTime = 0.0f;
				if (!currentAnimation->loop) {
					StopAnimation();
				}
			}
		}

			
		// Loop
		if (currentAnimation->loop && !currentAnimation->backwards && !currentAnimation->pingPong && !currentAnimation->easeIn && !currentAnimation->easeOut) {

			//Loop + Backwards
			if (currentAnimation->backwards) {
				currentTime = fmod(currentAnimation->GetDuration(), currentTime);
			}
			else {
				currentTime = fmod(currentTime, currentAnimation->GetDuration());
			}
		}

		if (currentTime < currentAnimation->GetDuration()) {
			CalculateBoneTransform(&currentAnimation->GetRootNode(), identity.identity);
		}

		float stepTime = currentTime + currentAnimation->GetTickPerSecond() * dt * currentAnimation->speed;

		if (stepTime > currentAnimation->GetDuration() && !currentAnimation->loop && !currentAnimation->pingPong) {
			//Leave animation in its final state
			currentTime = currentAnimation->GetDuration() - 0.01f;
			CalculateBoneTransform(&currentAnimation->GetRootNode(), identity.identity);

			currentTime = 0.0f;
			StopAnimation();
		}
	}
}

void Animator::PlayAnimation(Animation* animation)
{
	currentAnimation = animation; 
	currentTime = 0.0f; 
}

void Animator::PauseAnimation() {
	currentAnimation->isPlaying = false;
}

void Animator::ResumeAnimation()
{
	currentAnimation->isPlaying = true;
}

void Animator::StopAnimation()
{
	currentAnimation->isPlaying = false;
	currentTime = 0.0f;
	backwardsAux = true;
	pingPongAux = true;
	pingPongBackwardsAux = true;
	easeInSpeed = 1;
	easeOutSpeed = 1;
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
