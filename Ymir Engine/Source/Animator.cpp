#include "Animator.h"

#include "External/mmgr/mmgr.h"

Animator::Animator()
{
	currentAnimation = nullptr;

	currentAnimation = nullptr;
	finalBoneMatrices.reserve(100);
	for (int i = 0; i < 100; i++) {
		finalBoneMatrices.push_back(identity.identity);
	}
}

Animator::Animator(Animation* animation)
{
	currentAnimation = nullptr;

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
	
	//Current time test
	UpdateCurrentTime();

	for (int i = 0; i < animationsPlaying.size(); i++) {
		
		CalculateBoneTransform(&animationsPlaying[i]->animation->GetRootNode(), identity.identity,*animationsPlaying[i]);
	}

}

void Animator::UpdateCurrentTime() {

	for (int i = 0; i < animationsPlaying.size(); i++) {
		// Backwards
		if (animationsPlaying[i]->animation->backwards && !animationsPlaying[i]->animation->pingPong && !animationsPlaying[i]->animation->easeIn && !animationsPlaying[i]->animation->easeOut) {
			//Start from the last frame
			if (animationsPlaying[i]->animation->backwardsAux == true) {
				animationsPlaying[i]->currentTime = animationsPlaying[i]->animation->GetDuration();
				animationsPlaying[i]->animation->backwardsAux = false;
			}
			//If reached first frame, stop or go to last frame
			if (animationsPlaying[i]->currentTime < 0.0f) {

				if (animationsPlaying[i]->animation->loop) {
					animationsPlaying[i]->animation->backwardsAux = true;
				}
				else {
					if (!animationsPlaying[i]->animation->backwardsAux) {
						StopAnimation();
						ResetAnimation(i);
						animationsPlaying[i]->animation->backwardsAux = true;
					}
					animationsPlaying[i]->animation->backwardsAux = true;
				}
			}
			animationsPlaying[i]->currentTime -= animationsPlaying[i]->animation->GetTickPerSecond() * deltaTime * animationsPlaying[i]->animation->speed;
		}
		else if (!animationsPlaying[i]->animation->pingPong && !animationsPlaying[i]->animation->easeIn && !animationsPlaying[i]->animation->easeOut) {
			animationsPlaying[i]->currentTime += animationsPlaying[i]->animation->GetTickPerSecond() * deltaTime * animationsPlaying[i]->animation->speed;
		}

		//PingPong
		if (animationsPlaying[i]->animation->pingPong && !animationsPlaying[i]->animation->easeIn && !animationsPlaying[i]->animation->easeOut) {
			//PingPong and backwards both on
			if (animationsPlaying[i]->animation->pingPongBackwardsAux && animationsPlaying[i]->animation->backwards) {
				animationsPlaying[i]->currentTime = animationsPlaying[i]->animation->GetDuration();
				animationsPlaying[i]->animation->pingPongAux = false;
				animationsPlaying[i]->animation->pingPongBackwardsAux = false;
			}
			//Going
			if (animationsPlaying[i]->animation->pingPongAux) {
				animationsPlaying[i]->currentTime += animationsPlaying[i]->animation->GetTickPerSecond() * deltaTime * animationsPlaying[i]->animation->speed;
				if (animationsPlaying[i]->currentTime > animationsPlaying[i]->animation->GetDuration()) {
					animationsPlaying[i]->currentTime = animationsPlaying[i]->animation->GetDuration();
					animationsPlaying[i]->animation->pingPongAux = false;
					if (!animationsPlaying[i]->animation->loop && animationsPlaying[i]->animation->backwards) {
						animationsPlaying[i]->animation->pingPongBackwardsAux = true;
						animationsPlaying[i]->animation->pingPongAux = true;
						StopAnimation();
						ResetAnimation(i);
					}
				}
			}
			//Returning
			if (!animationsPlaying[i]->animation->pingPongAux) {
				animationsPlaying[i]->currentTime -= animationsPlaying[i]->animation->GetTickPerSecond() * deltaTime * animationsPlaying[i]->animation->speed;
				if (animationsPlaying[i]->currentTime < 0.0f) {
					animationsPlaying[i]->currentTime = 0.0f;
					animationsPlaying[i]->animation->pingPongAux = true;
					if (!animationsPlaying[i]->animation->loop && !animationsPlaying[i]->animation->backwards) {
						StopAnimation();
						ResetAnimation(i);
					}
				}
			}
		}

		//Ease-In
		if (animationsPlaying[i]->animation->easeIn) {
			animationsPlaying[i]->currentTime += animationsPlaying[i]->animation->GetTickPerSecond() * deltaTime * animationsPlaying[i]->animation->speed * animationsPlaying[i]->animation->easeInSpeed;
			animationsPlaying[i]->animation->easeInSpeed *= animationsPlaying[i]->animation->easeInMultiplier;
			if (animationsPlaying[i]->currentTime > animationsPlaying[i]->animation->GetDuration()) {
				animationsPlaying[i]->animation->easeInSpeed = 1;
				animationsPlaying[i]->currentTime = 0.0f;
				if (!animationsPlaying[i]->animation->loop) {
					StopAnimation();
					ResetAnimation(i);
				}
			}
		}

		//Ease-Out
		if (animationsPlaying[i]->animation->easeOut) {
			animationsPlaying[i]->currentTime += animationsPlaying[i]->animation->GetTickPerSecond() * deltaTime * animationsPlaying[i]->animation->speed * animationsPlaying[i]->animation->easeOutSpeed;
			animationsPlaying[i]->animation->easeOutSpeed *= animationsPlaying[i]->animation->easeOutMultiplier;
			if (animationsPlaying[i]->currentTime > animationsPlaying[i]->animation->GetDuration()) {
				animationsPlaying[i]->animation->easeOutSpeed = 1;
				animationsPlaying[i]->currentTime = 0.0f;
				if (!animationsPlaying[i]->animation->loop) {
					StopAnimation();
					ResetAnimation(i);
				}
			}
		}


		// Loop
		if (animationsPlaying[i]->animation->loop && !animationsPlaying[i]->animation->backwards && !animationsPlaying[i]->animation->pingPong && !animationsPlaying[i]->animation->easeIn && !animationsPlaying[i]->animation->easeOut) {

			//Loop + Backwards
			if (animationsPlaying[i]->animation->backwards) {
				animationsPlaying[i]->currentTime = fmod(animationsPlaying[i]->animation->GetDuration(), animationsPlaying[i]->currentTime);
			}
			else {
				animationsPlaying[i]->currentTime = fmod(animationsPlaying[i]->currentTime, animationsPlaying[i]->animation->GetDuration());
			}
		}

		if (animationsPlaying[i]->currentTime < animationsPlaying[i]->animation->GetDuration()) {
			//CalculateBoneTransform(&animationsPlaying[i]->animation->GetRootNode(), identity.identity);
		}

		float stepTime = animationsPlaying[i]->currentTime + animationsPlaying[i]->animation->GetTickPerSecond() * deltaTime * animationsPlaying[i]->animation->speed;

		if (stepTime > animationsPlaying[i]->animation->GetDuration() && !animationsPlaying[i]->animation->loop && !animationsPlaying[i]->animation->pingPong) {
			//Leave animation in its final state
			animationsPlaying[i]->currentTime = animationsPlaying[i]->animation->GetDuration() - 0.01f;
			//CalculateBoneTransform(&animationsPlaying[i]->animation->GetRootNode(), identity.identity);

			animationsPlaying[i]->currentTime = 0.0f;
			StopAnimation();
			ResetAnimation(i);
		}
	}
}

void Animator::PlayAnimation(Animation* animation)
{
	currentAnimation = animation; 
	AnimationController* temp = new AnimationController;
	temp->animation = animation;
	temp->currentTime = 0;
	animationsPlaying.push_back(temp);
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
	//currentTime = 0.0f;
}

void Animator::ResetAnimation(int ID) {
	animationsPlaying[ID]->currentTime = 0.0f;
	animationsPlaying[ID]->animation->backwards = true;
	animationsPlaying[ID]->animation->pingPongAux = true;
	animationsPlaying[ID]->animation->pingPongBackwardsAux = true;
	animationsPlaying[ID]->animation->easeInSpeed = 1;
	animationsPlaying[ID]->animation->easeOutSpeed = 1;
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, float4x4 parentTransform, AnimationController &animationController)
{
	std::string nodeName = node->name;
	float4x4 nodeTransform = node->transformation; 

	Bone* bone = animationController.animation->FindBone(nodeName);
	if (bone) {
		bone->Update(animationController.currentTime);
		nodeTransform = bone->GetLocalTransform();
	}

	float4x4 globalTransform = parentTransform * nodeTransform;

	std::map<std::string, BoneInfo> boneInfoMap = animationController.animation->GetBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
		int index = boneInfoMap[nodeName].id;
		float4x4 offset = boneInfoMap[nodeName].offset;
		finalBoneMatrices[index] = globalTransform * offset; 
	}

	for (int i = 0; i < node->childrenCount; i++) {
		CalculateBoneTransform(&node->children[i], globalTransform, animationController);
	}
}
