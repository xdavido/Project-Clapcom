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
	
	for (int i = 0; i < animationsPlaying.size(); i++) {
		
		CalculateBoneTransform(&animationsPlaying[i]->animation->GetRootNode(), identity.identity);
	}

	//if (currentAnimation) {

	//	// Backwards
	//	if (currentAnimation->backwards && !currentAnimation->pingPong && !currentAnimation->easeIn && !currentAnimation->easeOut) {
	//		//Start from the last frame
	//		if (backwardsAux == true) {
	//			currentTime = currentAnimation->GetDuration();
	//			backwardsAux = false;
	//		}
	//		//If reached first frame, stop or go to last frame
	//		if (currentTime < 0.0f) {

	//			if (currentAnimation->loop) {
	//				backwardsAux = true;
	//			}
	//			else {
	//				if (!backwardsAux) {
	//					StopAnimation();
	//					backwardsAux = true;
	//				}
	//				backwardsAux = true;
	//			}
	//		}
	//		currentTime -= currentAnimation->GetTickPerSecond() * dt * currentAnimation->speed;
	//	}
	//	else if (!currentAnimation->pingPong && !currentAnimation->easeIn && !currentAnimation->easeOut) {
	//		currentTime += currentAnimation->GetTickPerSecond() * dt * currentAnimation->speed;
	//	}

	//	//PingPong
	//	if (currentAnimation->pingPong && !currentAnimation->easeIn && !currentAnimation->easeOut) {
	//		//PingPong and backwards both on
	//		if (pingPongBackwardsAux && currentAnimation->backwards) {
	//			currentTime = currentAnimation->GetDuration();
	//			pingPongAux = false;
	//			pingPongBackwardsAux = false;
	//		}
	//		//Going
	//		if (pingPongAux) {
	//			currentTime += currentAnimation->GetTickPerSecond() * dt * currentAnimation->speed;
	//			if (currentTime > currentAnimation->GetDuration()) {
	//				currentTime = currentAnimation->GetDuration();
	//				pingPongAux = false;
	//				if (!currentAnimation->loop && currentAnimation->backwards) {
	//					pingPongBackwardsAux = true;
	//					pingPongAux = true;
	//					StopAnimation();
	//				}
	//			}
	//		}
	//		//Returning
	//		if (!pingPongAux) {
	//			currentTime -= currentAnimation->GetTickPerSecond() * dt * currentAnimation->speed;
	//			if (currentTime < 0.0f) {
	//				currentTime = 0.0f;
	//				pingPongAux = true;
	//				if (!currentAnimation->loop && !currentAnimation->backwards) {
	//					StopAnimation();
	//				}
	//			}
	//		}
	//	}

	//	//Ease-In
	//	if (currentAnimation->easeIn) {
	//		currentTime += currentAnimation->GetTickPerSecond() * dt * currentAnimation->speed * easeInSpeed;
	//		easeInSpeed *= easeInMultiplier;
	//		if (currentTime > currentAnimation->GetDuration()) {
	//			easeInSpeed = 1;
	//			currentTime = 0.0f;
	//			if (!currentAnimation->loop) {
	//				StopAnimation();
	//			}
	//		}
	//	}

	//	//Ease-Out
	//	if (currentAnimation->easeOut) {
	//		currentTime += currentAnimation->GetTickPerSecond() * dt * currentAnimation->speed * easeOutSpeed;
	//		easeOutSpeed *= easeOutMultiplier;
	//		if (currentTime > currentAnimation->GetDuration()) {
	//			easeOutSpeed = 1;
	//			currentTime = 0.0f;
	//			if (!currentAnimation->loop) {
	//				StopAnimation();
	//			}
	//		}
	//	}

	//		
	//	// Loop
	//	if (currentAnimation->loop && !currentAnimation->backwards && !currentAnimation->pingPong && !currentAnimation->easeIn && !currentAnimation->easeOut) {

	//		//Loop + Backwards
	//		if (currentAnimation->backwards) {
	//			currentTime = fmod(currentAnimation->GetDuration(), currentTime);
	//		}
	//		else {
	//			currentTime = fmod(currentTime, currentAnimation->GetDuration());
	//		}
	//	}

	//	if (currentTime < currentAnimation->GetDuration()) {
	//		CalculateBoneTransform(&currentAnimation->GetRootNode(), identity.identity);
	//	}

	//	float stepTime = currentTime + currentAnimation->GetTickPerSecond() * dt * currentAnimation->speed;

	//	if (stepTime > currentAnimation->GetDuration() && !currentAnimation->loop && !currentAnimation->pingPong) {
	//		//Leave animation in its final state
	//		currentTime = currentAnimation->GetDuration() - 0.01f;
	//		CalculateBoneTransform(&currentAnimation->GetRootNode(), identity.identity);

	//		currentTime = 0.0f;
	//		StopAnimation();
	//	}
	//}
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
			CalculateBoneTransform(&animationsPlaying[i]->animation->GetRootNode(), identity.identity);
		}

		float stepTime = animationsPlaying[i]->currentTime + animationsPlaying[i]->animation->GetTickPerSecond() * deltaTime * animationsPlaying[i]->animation->speed;

		if (stepTime > animationsPlaying[i]->animation->GetDuration() && !animationsPlaying[i]->animation->loop && !animationsPlaying[i]->animation->pingPong) {
			//Leave animation in its final state
			animationsPlaying[i]->currentTime = animationsPlaying[i]->animation->GetDuration() - 0.01f;
			CalculateBoneTransform(&animationsPlaying[i]->animation->GetRootNode(), identity.identity);

			animationsPlaying[i]->currentTime = 0.0f;
			StopAnimation();
			ResetAnimation(i);
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
