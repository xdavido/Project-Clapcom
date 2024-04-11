#include "Animator.h"
#include "Log.h"
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
	
	if (currentAnimation && currentAnimation->isPlaying) {
		UpdateCurrentTime(currentAnimation);
		CalculateBoneTransform(&currentAnimation->GetRootNode(), identity.identity);
	}

	// Blending
	if (previousAnimation != nullptr
		&& CheckBlendMap(previousAnimation, currentAnimation->name)
		&& transitionTime < previousAnimation->blendMap.at(currentAnimation->name)) {

		transitionTime += previousAnimation->GetTickPerSecond() * deltaTime;
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
					StopAnimation();
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
					StopAnimation();
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
					StopAnimation();
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
				StopAnimation();
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
				StopAnimation();
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

	float stepTime = animation->currentTime + animation->GetTickPerSecond() * deltaTime * animation->speed;

	if (stepTime > animation->GetDuration() && !animation->loop && !animation->pingPong) {

		animation->isPlaying = false;

		if (animation->resetToZero) {
			currentAnimation->currentTime = 0.0f;
			CalculateBoneTransform(&currentAnimation->GetRootNode(), identity.identity);
		}
		else {
			//Leave animation in its final state
			animation->currentTime = animation->GetDuration() - 0.01f;
		}

		ResetAnimation(animation);
	}
		
		
}

void Animator::PlayAnimation(ResourceAnimation* animation)
{
	if (previousAnimation) {
		lastCurrentTime = previousAnimation->currentTime;
		LOG("PrevAnim Time: %f", lastCurrentTime);
	}

	previousAnimation = currentAnimation;
	currentAnimation = animation;
	
	currentAnimation->isPlaying = true;
	currentAnimation->currentTime = 0.0f;

	transitionTime = .0f;
}

void Animator::PauseAnimation() {

	currentAnimation->isPlaying = false;
}

void Animator::ResumeAnimation() {

	currentAnimation->isPlaying = true;
}

void Animator::StopAnimation() {

	currentAnimation->isPlaying = false;
	currentAnimation->currentTime = 0.0f;
	CalculateBoneTransform(&currentAnimation->GetRootNode(), identity.identity);
}

void Animator::ResetAnimation(ResourceAnimation* animation) {

	animation->backwardsAux = true;
	animation->pingPongAux = true;
	animation->pingPongBackwardsAux = true;
	//animation->easeInSpeed = 1;
	//animation->easeOutSpeed = 1;
}

float Animator::CalculatePreviousTime(ResourceAnimation* lastAnimation, float transitionTime) {

	float time = lastCurrentTime + transitionTime;

	if (time > lastAnimation->duration) {
		float timeDecimals = time - (int)time; // Illegal code
		time = (int)time % (int)lastAnimation->duration;
		time += timeDecimals;

		return time;
	}
}

bool Animator::CheckBlendMap(ResourceAnimation* animation, std::string animationBlend) {

	if (animation->blendMap.find(animationBlend) == animation->blendMap.end()) return false;
	return true;
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, float4x4 parentTransform)
{
	std::string nodeName = node->name;
	float4x4 nodeTransform = node->transformation; 

	Bone* bone = currentAnimation->FindBone(nodeName);
	if (bone) {
		bone->Update(currentAnimation->currentTime);
		nodeTransform = bone->GetLocalTransform();
	}

	// Blending
	if (previousAnimation != nullptr 
		&& CheckBlendMap(previousAnimation, currentAnimation->name) 
		&& transitionTime < previousAnimation->blendMap.at(currentAnimation->name)) {

		float4x4 prevTransform = node->transformation;

		Bone* prevBone = previousAnimation->FindBone(nodeName);
		if (prevBone) {
			prevBone->Update(CalculatePreviousTime(previousAnimation, transitionTime));
			prevTransform = prevBone->GetLocalTransform();
			prevTransform = prevTransform;
		}

		float3 translate = float3(.0f, .0f, .0f);;
		Quat rotation = Quat::identity;;
		float3 scale = float3(1, 1, 1);;

		float3 prevTranslate = float3(.0f,.0f,.0f);
		Quat prevRotation = Quat::identity;
		float3 prevScale = float3(1,1,1);

		nodeTransform.Decompose(translate, rotation, scale);
		prevTransform.Decompose(prevTranslate, prevRotation, prevScale);

		// Calculate lerp value
		
		float lerpValue = 1 - (transitionTime / previousAnimation->blendMap.at(currentAnimation->name));

		translate = translate.Lerp(prevTranslate, lerpValue);
		rotation = rotation.Slerp(prevRotation, lerpValue);
		scale = scale.Lerp(prevScale, lerpValue);

		nodeTransform.SetRotatePart(rotation.ToFloat3x3());
		nodeTransform.Scale(scale);
		nodeTransform.SetTranslatePart(translate);
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

bool Animator::FindAnimation(std::string aniationName) {

	for (int i = 0; i < animations.size(); i++) {
		if (animations[i].name == aniationName) {
			return true;;
		} 
	}

	return false;
}
