#include "AnimationController.h"

#include "External/mmgr/mmgr.h"

AnimationController::AnimationController()
{

}

AnimationController::~AnimationController()
{

}

void AnimationController::AddAnimation()
{
	animations.push_back(0);
}

void AnimationController::AddAnimation(uint64 animationID)
{
	animations.push_back(animationID);
}